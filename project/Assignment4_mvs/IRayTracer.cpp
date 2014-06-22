#include <cassert>

#include "Constants.h"
#include "IRayTracer.h"
#include "Scene.h"

const Scene *IRayTracer::getScene() const {
	// Return the scene pointer
	return this->scene;
}

void IRayTracer::setScene(const Scene *scene) {
	// Set the new scene pointer
	this->scene = scene;
}

Vec3Df IRayTracer::performRayTracing(const Vec3Df &origin, const Vec3Df &dir) const {
	float distance;

	return this->performRayTracingIteration(origin, dir, 0, distance);
}