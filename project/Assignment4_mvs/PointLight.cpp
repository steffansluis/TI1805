#include <algorithm>

#include "PointLight.h"

PointLight::PointLight()
: position(), color(1, 1, 1) {
}

PointLight::PointLight(const Vec3Df &position)
: position(position), color(1, 1, 1) {
}

PointLight::PointLight(const Vec3Df &position, const Vec3Df &color)
: position(position), color(color) {
}

Vec3Df PointLight::getColor() const {
	return this->color;
}

void PointLight::setColor(const Vec3Df &color) {
	this->color = color;
}

bool PointLight::sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const {
	// Set the light point to the position of the light
	lightPoint = this->position;

	// Calculate the distance from the light to the point in the scene
	float distance = (point - lightPoint).getLength();

	// Set the light's color
	lightColor = this->getColor() * this->calculateIntensity(distance);

	// Realisticly this should return false 50% of the time
	return true;
}