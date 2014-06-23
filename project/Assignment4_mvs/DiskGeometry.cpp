#include <cassert>

#include "DiskGeometry.h"
#include "Random.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

DiskGeometry::DiskGeometry(const Vec3Df &normal, const Vec3Df &center, float radius)
: PlaneGeometry(normal, center), center(center), radius(radius)
{
	assert(radius > 0.0f);
}

float DiskGeometry::getArea() const {
	return Constants::Pi * this->radius * this->radius;
}

bool DiskGeometry::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	bool result = PlaneGeometry::calculateClosestIntersection(origin, dir, intersection);

	if (result) {
		// Check if the hitPoint is inside the disk
		return (intersection.hitPoint - this->center).getSquaredLength() <= (this->radius * this->radius);
	}

	return false;
}

void DiskGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	// Get two vectors orthogonal to the normal
	Vec3Df u, v;
	this->normal.getTwoOrthogonals(u, v);

	// Generate two random numbers
	float x, y;
	Random::sampleUnitDisk(x, y);

	// Offset the center with the two orthogonal vectors scaled by the random numbers
	Vec3Df point = this->center + x * u + y * v;

	surface.geometry = this->shared_from_this();
	surface.normal = this->normal;
	surface.point = point;
	surface.texCoords = Vec2Df(x, y);
	surface.isInside = false;
}

BoundingBox DiskGeometry::getBoundingBox() const {
	// Radius is a scalar so we need to make it into a Vec3Df
	Vec3Df radius = Vec3Df(this->radius, this->radius, this->radius);

	// Compute the center of the disk
	Vec3Df center = this->normal * this->distance;

	return BoundingBox(center - radius, center + radius);
}