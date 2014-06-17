#include <cassert>

#include "Image.h"
#include "IAccelerationStructure.h"
#include "ICamera.h"
#include "IGeometry.h"
#include "ILight.h"
#include "IRayTracer.h"
#include "NoAccelerationStructure.h"
#include "RayIntersection.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Vec3D.h"

Scene::Scene() {
	// Initialize the geometry and light vectors
	this->geometry = std::make_shared<std::vector<std::shared_ptr<IGeometry>>>();
	this->lights = std::make_shared<std::vector<std::shared_ptr<ILight>>>();

	// Set the acceleration structure
	this->setAccelerationStructure(std::make_shared<NoAccelerationStructure>());

	// Set the default ray tracer
	this->setRayTracer(std::make_shared<RayTracer>());
}

Scene::~Scene() {
}

std::shared_ptr<const RayIntersection> Scene::calculateIntersection(const Vec3Df & origin, const Vec3Df & dest) const {
	// Delegate the intersection calculations to the acceleration structure
	return this->accelerator->calculateIntersection(origin, dest);
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

	// Set the ray tracer and set its scene pointer to the current scene
	this->rayTracer = rayTracer;
	this->rayTracer->setScene(this->shared_from_this());
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

	// Iterate through each pixel
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vec3Df origin;
			Vec3Df dir;

			// Get the ray from the camera through the current pixel
			camera->getRay(x, y, origin, dir);

			// Trace the ray through the scene
			Vec3Df color = this->rayTracer->performRayTracing(origin, dir);

			// Set the resulting color in the image
			result->setPixel(x, y, RGBValue(color[0], color[1], color[2]));
		}

		std::cout << "Row " << y << std::endl;
	}

	std::cout << "Done!" << std::endl;

	return result;
}

void Scene::preprocess() {
	// Preprocess the acceleration structure
	this->accelerator->preprocess();

	// Preprocess all geometry
	for (std::vector<std::shared_ptr<IGeometry>>::iterator it = this->geometry->begin(); it != this->geometry->end(); ++it) {
		(*it)->preprocess();
	}

	// Preprocess all lights
	for (std::vector<std::shared_ptr<ILight>>::iterator it = this->lights->begin(); it != this->lights->end(); ++it) {
		(*it)->preprocess();
	}
}