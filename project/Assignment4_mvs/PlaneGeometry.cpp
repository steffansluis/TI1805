#include <limits>

#include "PlaneGeometry.h"
#include "Random.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

PlaneGeometry::PlaneGeometry(const Vec3Df &normal, float distance) 
: normal(normal), distance(distance) {
}

float PlaneGeometry::getArea() const {
	return std::numeric_limits<float>::infinity();
}

bool PlaneGeometry::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	Vec3Df p = this->normal * this->distance;

	float t = Vec3Df::dotProduct(p - origin, -this->normal) / Vec3Df::dotProduct(dir, -this->normal);

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
}

void PlaneGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	float u, v;
	Vec3Df uv, vv;
	Random::sampleUnitSquare(u, v);
	this->normal.getTwoOrthogonals(uv, vv);

	surface.geometry = this->shared_from_this();
	surface.normal = this->normal;
	surface.point = this->normal * this->distance + u * uv + v * vv;
	surface.texCoords = Vec2Df(u, v);
}

BoundingBox PlaneGeometry::getBoundingBox() const {
	return BoundingBox();
}