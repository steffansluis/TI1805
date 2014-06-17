#include <cassert>

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