#include <cassert>

#include "AreaLight.h"
#include "IGeometry.h"
#include "SurfacePoint.h"

AreaLight::AreaLight(std::shared_ptr<IGeometry> geometry) {
	assert(geometry);

	this->setGeometry(geometry);
}

void AreaLight::sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const {
	SurfacePoint surface;
	Vec3Df lightVector;

	// Sample a random point on the surface
	this->getGeometry()->getRandomSurfacePoint(surface);

	// Set the light point
	lightPoint = surface.point;

	// Calculate the light vector and distance
	lightVector = point - lightPoint;
	float distance = lightVector.normalize();

	// Set the light's color
	lightColor = surface.emittedLight(lightVector) * this->calculateIntensity(distance);
}