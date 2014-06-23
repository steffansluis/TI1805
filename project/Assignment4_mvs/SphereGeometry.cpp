#include <algorithm>
#include <cassert>
#include <cmath>	

#include "Constants.h"
#include "Random.h"
#include "RayIntersection.h"
#include "SphereGeometry.h"
#include "SurfacePoint.h"

SphereGeometry::SphereGeometry(const Vec3Df &position, float radius)
: position(position), radius(radius) {
	assert(radius > 0);

	// Nothing to do here
}

float SphereGeometry::getArea() const {
	// Area of a sphere is 4pi * r^2
	return 4.0f * Constants::Pi * this->radius * this->radius;
}

bool SphereGeometry::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	// Ray origin relative to the sphere
	Vec3Df local = origin - this->position;

	// Calculate the a, b and c constants of the quadratic equation
	float a = Vec3Df::dotProduct(dir, dir);
	float b = 2.0f * Vec3Df::dotProduct(dir, local);
	float c = Vec3Df::dotProduct(local, local) - this->radius * this->radius;

	// Calculate the determinant
	float determinant = b * b - 4.0f * a * c;

	// If the determinant is negative, the ray missed
	if (determinant < 0.0f)
		return false;

	// Square root of the determinant is delayed for efficiency (saves a square root if the ray misses) 
	determinant = sqrtf(determinant);

	// We deviate slightly from the standard way of solving quadratic formulas,
	// because it leads to better numerical precision in certain cases according to:
	// http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection#Solving_the_Quadratic_Equation
	float q;
	if (b < 0.0f)
		q = (-b + determinant) * 0.5f;
	else
		q = (-b - determinant) * 0.5f;

	float tmin = q / a;
	float tmax = c / q;

	// Check whether the intersection was on the inside or
	// outside of the sphere and set the parameters accordingly
	if (tmin <= 0.0f || tmax < tmin) {
		intersection.distance = tmax;
		intersection.isInside = false;
	}
	else {
		intersection.distance = tmin;
		intersection.isInside = true;
	}

	// If the distance is negative the intersection occured behind the ray
	if (intersection.distance <= 0.0f)
		return false;

	// Set the intersection parameters
	intersection.geometry = this->shared_from_this();
	intersection.origin = origin;
	intersection.direction = dir;
	intersection.hitPoint = origin + dir * intersection.distance;

	return true;
}

void SphereGeometry::getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const {
	surface.geometry = intersection.geometry;
	surface.point = intersection.hitPoint;
	surface.normal = intersection.hitPoint - this->position;
	surface.normal.normalize();
	surface.texCoords = this->getTextureCoordinates(surface.point);
	surface.isInside = intersection.isInside;

	// Flip the normal if the intersection occured on the inside of the primitive
	if (intersection.isInside) {
		surface.normal = -surface.normal;
	}
}

void SphereGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	surface.geometry = this->shared_from_this();
	surface.point = Random::sampleUnitSphere() * this->radius + this->position;
	surface.normal = surface.point - this->position;
	surface.normal.normalize();
	surface.texCoords = this->getTextureCoordinates(surface.point);
	surface.isInside = false;
}

BoundingBox SphereGeometry::getBoundingBox() const {
	// Radius is a scalar so we need to make it into a Vec3Df
	Vec3Df radius = Vec3Df(this->radius, this->radius, this->radius);

	return BoundingBox(this->position - radius, this->position + radius);
}

Vec2Df SphereGeometry::getTextureCoordinates(const Vec3Df &point) const {
	// Calculate a vector from the point to the sphere's origin
	Vec3Df d = this->position - point;
	d.normalize();

	// Calculate the UV coordinates
	// http://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
	return Vec2Df(
		0.5f + atan2f(d[2], d[0]) / Constants::TwoPi,
		0.5f - asinf(d[1]) / Constants::Pi
		);
}