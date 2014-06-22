#include <cassert>

#include "AreaLight.h"
#include "IGeometry.h"
#include "SurfacePoint.h"

AreaLight::AreaLight(std::shared_ptr<IGeometry> geometry) {
	assert(geometry);

	this->setGeometry(geometry);
}

bool AreaLight::sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const {
	SurfacePoint surface;
	Vec3Df lightVector;

	// Sample a random point on the surface
	this->getGeometry()->getRandomSurfacePoint(surface);

	// Set the light point
	lightPoint = surface.point;

	// Calculate the light vector and distance
	lightVector = point - lightPoint;
	float distance = lightVector.normalize();

	// If the [point is on the wrong side of the surface return false
	if (Vec3Df::dotProduct(surface.normal, lightVector) <= 0.0f)
		return false;

	// Set the light's color
	lightColor = surface.emittedLight(lightVector) * this->calculateIntensity(distance);

	return true;
}