#include <cassert>

#include "Image.h"
#include "IAccelerationStructure.h"
#include "ICamera.h"
#include "IGeometry.h"
#include "ILight.h"
#include "IRayTracer.h"
#include "NoAccelerationStructure.h"
#include "BTreeAccelerator.h"
#include "RayIntersection.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Vec3D.h"

Scene::Scene() :
geometry(std::make_shared<std::vector<std::shared_ptr<IGeometry>>>()),
lights(std::make_shared<std::vector<std::shared_ptr<ILight>>>()),
lightSampleDensity(10.0f) {
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

void Scene::setAmbientLight(const Vec3Df& ambientLight) {
	this->ambientLight = ambientLight;
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

	// Iterate through each pixel
	printf("beginrender");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++, iterationCounter++) {
			Vec3Df origin;
			Vec3Df dir;

#if _DEBUG	// _DEBUG is probably not defined in GCC
			// print the iteration
			if (iterationCounter % 100 == 0)
			{
				std::cout << "iteration: " << iterationCounter << "\n";
			}
#endif
			// Get a number of rays per pixel for the depth of field effect.
			float raysPerPixel = 25;
			float red=0;
			float green=0;
			float blue=0;
			Vec3Df color;
			for (int i = 0; i < raysPerPixel; i++){
				// Get the ray from the camera through the current pixel
				camera->getRay(x, y, origin, dir);
				Vec3Df color = this->rayTracer->performRayTracing(origin, dir);
				red += color[0];
				green += color[1];
				blue += color[2];
				printf("depthopfieldray");

			}
			red /= raysPerPixel;
			green /= raysPerPixel;
			blue /= raysPerPixel;
			result->setPixel(x, y, RGBValue(red, green, blue));
			// Set the resulting color in the image
			
		}

		std::cout << "Row " << y << std::endl;
	}

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