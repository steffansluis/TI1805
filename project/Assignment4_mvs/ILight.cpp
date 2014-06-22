#include <algorithm>

#include "IGeometry.h"
#include "ILight.h"

ILight::ILight()
: falloff(0), intensity(1) {
}

ILight::~ILight() {
}

float ILight::getArea() const {
	if (this->geometry)
		return this->geometry->getArea();
	else
		return 1.0f;
}

std::shared_ptr<IGeometry> ILight::getGeometry() const {
	return this->geometry;
}

float ILight::getFalloff() const {
	return this->falloff;
}

float ILight::getIntensity() const {
	return this->intensity;
}

void ILight::setGeometry(std::shared_ptr<IGeometry> geometry) {
	this->geometry = geometry;
}

void ILight::setFalloff(float falloff) {
	this->falloff = falloff;
}

void ILight::setIntensity(float intensity) {
	this->intensity = intensity;
}

void ILight::preprocess() {
}

float ILight::calculateAttenuation(float distance) const {
	// Calculate the intensity falloff over distance
	float attenuation = 1.0f - this->falloff * distance * distance;

	// Make sure it does not go negative
	return std::max<float>(0.0f, attenuation);
}

float ILight::calculateIntensity(float distance) const {
	return this->intensity / this->getArea() * this->calculateAttenuation(distance);
}