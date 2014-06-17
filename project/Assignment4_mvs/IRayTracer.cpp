#include <cassert>

#include "IRayTracer.h"
#include "Scene.h"

std::shared_ptr<const Scene> IRayTracer::getScene() const {
	// Return the scene pointer
	return this->scene;
}

void IRayTracer::setScene(std::shared_ptr<const Scene> scene) {
	assert(scene);

	// Set the new scene pointer
	this->scene = scene;
}