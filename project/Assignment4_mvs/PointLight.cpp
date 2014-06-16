#include "PointLight.h"

PointLight::PointLight(const Vec3Df &position, const Vec3Df &color)
: position(position), color(color) {
}

Vec3Df PointLight::getLightTowards(const Vec3Df &point, Vec3Df &outgoingVector) {
	// Calculate the vector from the light's position to the point in the scene
	outgoingVector = point - this->position;
	outgoingVector.normalize();

	// Return the light's color
	return this->color;
}