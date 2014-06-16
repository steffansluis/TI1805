#include <assert.h>

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
	// Set the acceleration structure
	this->setAccelerationStructure(new NoAccelerationStructure());

	// Set the default ray tracer
	this->setRayTracer(new RayTracer());

	// Initialize the geometry and light vectors
	this->geometry = std::vector<IGeometry*>();
	this->lights = std::vector<ILight*>();
}

Scene::~Scene() {
}

RayIntersection *Scene::calculateIntersection(const Vec3Df & origin, const Vec3Df & dest) const {
	// Delegate the intersection calculations to the acceleration structure
	return this->accelerator->calculateIntersection(origin, dest);
}

void Scene::addGeometry(IGeometry *geometry) {
	assert(geometry);

	// Add the geometry to the vector
	this->geometry.push_back(geometry);
}

void Scene::addLight(ILight *light) {
	assert(light);

	// Add the light to the vector
	this->lights.push_back(light);
}

const std::vector<IGeometry*> *Scene::getGeometry() const {
	// Return a pointer to the geometry vector
	return &this->geometry;
}

const std::vector<ILight*> *Scene::getLights() const {
	// Return a pointer to the light vector
	return &this->lights;
}

IAccelerationStructure *Scene::getAccelerationStructure() const {
	// Return the pointer to the acceleration structure
	return this->accelerator;
}

IAccelerationStructure *Scene::setAccelerationStructure(IAccelerationStructure *accelerator) {
	assert(accelerator);

	// Store the old acceleration structure pointer
	IAccelerationStructure *oldAccelerator = this->accelerator;

	// Set the acceleration structure and set its geometry vector to the scene's geometry
	this->accelerator = accelerator;
	this->accelerator->setGeometry(&this->geometry);

	// Return the pointer to the old acceleration structure
	return oldAccelerator;
}

IRayTracer *Scene::getRayTracer() const {
	// Returns the pointer to the ray tracer
	return this->rayTracer;
}

IRayTracer *Scene::setRayTracer(IRayTracer *rayTracer) {
	assert(rayTracer);

	// Store the old ray tracer pointer
	IRayTracer *oldRayTracer = this->rayTracer;

	// Set the ray tracer and set its scene pointer to the current scene
	this->rayTracer = rayTracer;
	this->rayTracer->setScene(this);

	// Return the pointer to the old ray tracer
	return oldRayTracer;
}

Image *Scene::render(ICamera *camera, int width, int height) {
	assert(camera);
	assert(width > 0);
	assert(height > 0);

	// Preprocess the scene
	this->preprocess();

	// Preprocess the camera
	camera->preprocess(width, height);

	// Create an image
	Image *result = new Image(width, height);

	// Iterate through each pixel
	// !!! Only iterating through centre right now to speed things up
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
	for (std::vector<IGeometry*>::iterator it = this->geometry.begin(); it != this->geometry.end(); ++it) {
		(*it)->preprocess();
	}

	// Preprocess all lights
	for (std::vector<ILight*>::iterator it = this->lights.begin(); it != this->lights.end(); ++it) {
		(*it)->preprocess();
	}
}