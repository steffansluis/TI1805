#include <assert.h>

#include "IRayTracer.h"
#include "Scene.h"

const Scene *IRayTracer::getScene() const {
	// Return the scene pointer
	return this->scene;
}

const Scene *IRayTracer::setScene(const Scene *scene) {
	assert(scene);

	// Store the old scene pointer
	const Scene *oldScene = this->scene;

	// Set the new scene pointer
	this->scene = scene;

	// Return the old scene pointer
	return oldScene;
}