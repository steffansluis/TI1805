#include <limits>

#include "PlaneGeometry.h"
#include "Random.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

PlaneGeometry::PlaneGeometry(const Vec3Df &normal, float distance)
: normal(normal), distance(distance) {
}

PlaneGeometry::PlaneGeometry(const Vec3Df &normal, const Vec3Df &point)
: normal(normal), distance(Vec3Df::dotProduct(normal, point)) {
}

float PlaneGeometry::getArea() const {
	return std::numeric_limits<float>::infinity();
}

bool PlaneGeometry::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	Vec3Df p = this->normal * this->distance;

	float dir_dot_normal = Vec3Df::dotProduct(dir, this->normal);

	if (dir_dot_normal == 0.0f) {
		return false;
	}
	else if (dir_dot_normal > 0.0f) {
		intersection.isInside = true;
	}
	else {
		intersection.isInside = false;
	}

	float t = Vec3Df::dotProduct(p - origin, this->normal) / dir_dot_normal;

	if (t < 0.0f) {
		return false;
	}

	intersection.geometry = this->shared_from_this();
	intersection.distance = t;
	intersection.direction = dir;
	intersection.origin = origin;
	intersection.hitPoint = origin + t * dir;

	return true;
}

void PlaneGeometry::getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const {
	// TODO: Project point onto normal for texCoords
	surface.geometry = intersection.geometry;
	surface.normal = this->normal;
	surface.point = intersection.hitPoint;
	surface.texCoords = Vec2Df();
	surface.isInside = intersection.isInside;

	// Flip the normal if the intersection occured on the inside of the primitive
	if (intersection.isInside) {
		surface.normal = -surface.normal;
	}
}

void PlaneGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	// Get two vectors orthogonal to the normal
	Vec3Df u, v;
	this->normal.getTwoOrthogonals(u, v);

	// Generate two random numbers
	float x, y;
	Random::sampleUnitSquare(x, y);

	// Find a point on the plane
	Vec3Df point = this->normal * distance;

	// Offset the point with the two orthogonal vectors scaled by the random numbers
	point += x * v + y * v;

	surface.geometry = this->shared_from_this();
	surface.normal = this->normal;
	surface.point = point;
	surface.texCoords = Vec2Df(x, y);
	surface.isInside = false;
}

BoundingBox PlaneGeometry::getBoundingBox() const {
	return BoundingBox();
}