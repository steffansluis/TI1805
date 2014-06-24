#include <cassert>
#include <cstdlib>
#include <ctime>
#include <omp.h>

#include "BTreeAccelerator.h"
#include "Image.h"
#include "IAccelerationStructure.h"
#include "ICamera.h"
#include "IGeometry.h"
#include "ILight.h"
#include "IRayTracer.h"
#include "NoAccelerationStructure.h"
#include "Random.h"
#include "RayIntersection.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Vec3D.h"

Scene::Scene() :
geometry(std::make_shared<std::vector<std::shared_ptr<IGeometry>>>()),
lights(std::make_shared<std::vector<std::shared_ptr<ILight>>>()),
lightSampleDensity(1.0f),
samplesPerPixel(1),
ambientOcclusionSamples(0),
maxTraceDepth(4),
pathTracingEnabled(false)
{
	// Set the acceleration structure
	this->setAccelerationStructure(std::make_shared<NoAccelerationStructure>());

	// Set the default ray tracer
	this->setRayTracer(std::make_shared<RayTracer>());
}

Scene::~Scene() {
}

bool Scene::calculateClosestIntersection(const Vec3Df & origin, const Vec3Df & dir, RayIntersection &intersection) const {
	// Delegate the intersection calculations to the acceleration structure
	return this->accelerator->calculateClosestIntersection(origin, dir, intersection);
}

bool Scene::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	// Delegate the intersection calculations to the acceleration structure
	return this->accelerator->calculateAnyIntersection(origin, dir, maxDistance, intersection);
}

void Scene::addGeometry(std::shared_ptr<IGeometry> geometry) {
	assert(geometry);

	// Add the geometry to the vector
	this->geometry->push_back(geometry);
}

void Scene::addLight(std::shared_ptr<ILight> light) {
	assert(light);

	// Add the light to the vector
	this->lights->push_back(light);
}

std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>>Scene::getGeometry() const {
	// Return a pointer to the geometry vector
	return this->geometry;
}

std::shared_ptr<const std::vector<std::shared_ptr<ILight>>> Scene::getLights() const {
	// Return a pointer to the light vector
	return this->lights;
}

std::shared_ptr<const IAccelerationStructure> Scene::getAccelerationStructure() const {
	// Return the pointer to the acceleration structure
	return this->accelerator;
}

float Scene::getLightSampleDensity() const {
	return this->lightSampleDensity;
}

Vec3Df Scene::getAmbientLight() const {
	return this->ambientLight;
}

bool Scene::getPathTracingEnabled() const {
	return this->pathTracingEnabled;
}

int Scene::getAmbientOcclusionSamples() const {
	return this->ambientOcclusionSamples;
}

int Scene::getSamplesPerPixel() const {
	return this->samplesPerPixel;
}

int Scene::getMaxTraceDepth() const {
	return this->maxTraceDepth;
}

void Scene::setAccelerationStructure(std::shared_ptr<IAccelerationStructure> accelerator) {
	assert(accelerator);
	
	// Set the acceleration structure and set its geometry vector to the scene's geometry
	this->accelerator = accelerator;
	this->accelerator->setGeometry(this->geometry);
}

std::shared_ptr<const IRayTracer> Scene::getRayTracer() const {
	// Returns the pointer to the ray tracer
	return this->rayTracer;
}

void Scene::setRayTracer(std::shared_ptr<IRayTracer> rayTracer) {
	assert(rayTracer);

	if (this->rayTracer)
		this->rayTracer->setScene(NULL);

	// Set the ray tracer and set its scene pointer to the current scene
	this->rayTracer = rayTracer;
	this->rayTracer->setScene(this);
}

void Scene::setLightSampleDensity(float density) {
	assert(density >= 0.0f);

	this->lightSampleDensity = density;
}

void Scene::setPathTracingEnabled(bool enabled) {
	this->pathTracingEnabled = enabled;
}

void Scene::setAmbientLight(const Vec3Df& ambientLight) {
	this->ambientLight = ambientLight;
}

void Scene::setAmbientOcclusionSamples(int numSamples) {
	assert(numSamples >= 0);

	this->ambientOcclusionSamples = numSamples;
}

void Scene::setSamplesPerPixel(int numSamples) {
	assert(numSamples >= 1);

	this->samplesPerPixel = numSamples;
}

void Scene::setMaxTraceDepth(int maxDepth) {
	assert(maxDepth >= 1);

	this->maxTraceDepth = maxDepth;
}

std::shared_ptr<Image> Scene::render(std::shared_ptr<ICamera> camera, int width, int height) {
	assert(camera);
	assert(width > 0);
	assert(height > 0);

	// Preprocess the scene
	this->preprocess();

	// Preprocess the camera
	camera->preprocess(width, height);

	// Create an image
	auto result = std::make_shared<Image>(width, height);

	// A counter for the iteration of the ray-tracing algorithm. 
	int iterationCounter = 0;

	clock_t start = clock();

#pragma omp parallel shared(camera, result)
	{
		// Set the random seed for each thread
		srand(time(NULL) ^ omp_get_thread_num());

		// Iterate through each pixel, collapse the calculation into separate threads if not on windows (seems like MVC doesnt support collapse)
#ifdef WIN32
#pragma omp for schedule(dynamic)
#else
#pragma omp for reduction(+:iterationCounter) collapse(2) schedule(dynamic)
#endif
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// Render the pixel
				Vec3Df color = this->renderPixel(camera, x, y);

				// Set the resulting color in the image
				result->setPixel(x, y, RGBValue(color[0], color[1], color[2]));

				// Printing is sloooow
				if (iterationCounter++ % width == 0)
				std::cout << "Pixel: " << iterationCounter << " / " << (width * height) << std::endl;
			}
		}
	}

	clock_t end = clock();
	std::cout << "Time: " << (end - start) / (double)CLOCKS_PER_SEC;

	std::cout << "Done!" << std::endl;

	return result;
}

void Scene::preprocess() {
	// Preprocess all geometry
	for (std::vector<std::shared_ptr<IGeometry>>::iterator it = this->geometry->begin(); it != this->geometry->end(); ++it) {
		(*it)->preprocess();
	}

	// Preprocess all lights
	for (std::vector<std::shared_ptr<ILight>>::iterator it = this->lights->begin(); it != this->lights->end(); ++it) {
		(*it)->preprocess();
	}

	// Preprocess the acceleration structure
	this->accelerator->preprocess();
}

Vec3Df Scene::renderPixel(std::shared_ptr<ICamera> camera, int x, int y) {
	Vec3Df result = Vec3Df();

	int samples = this->samplesPerPixel;

	// Stratisifed sampling
	for (int i = 0; i < samples; i++){
		for (int j = 0; j < samples; j++) {
			float u, v;
			Random::sampleUnitSquare(u, v);
			Vec3Df origin;
			Vec3Df dir;

			// Get the ray from the camera through the current pixel
			camera->getRay(x, y, (i + u) / (float)samples, (j + v) / (float)samples, origin, dir);

			// Trace the ray through the scene
			result += this->rayTracer->performRayTracing(origin, dir);
		}
	}

	return result / (float)(samples * samples);
}