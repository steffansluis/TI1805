#include <algorithm>

#include "PointLight.h"

PointLight::PointLight(const Vec3Df &position, const Vec3Df &color)
: position(position), color(color) {
}

void PointLight::sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const {
	// Set the light point to the position of the light
	lightPoint = this->position;

	// Calculate the distance from the light to the point in the scene
	float distance = (point - lightPoint).getLength();

	// Calculate the amount of attenuation
	float attenuation = this->calculateAttenuation(distance);

	// Set the light's color
	lightColor = this->color * this->getIntensity() * attenuation;
}