#include <algorithm>
#include <cassert>
#include <cmath>	

#include "Random.h"
#include "RayIntersection.h"
#include "SphereGeometry.h"
#include "SurfacePoint.h"

SphereGeometry::SphereGeometry(const Vec3Df &position, float radius) 
: position(position), radius(radius) {
	assert(radius > 0);
	
	// Nothing to do here
}


float SphereGeometry::getArea() const
{
	// DUMMY VALUE
	return 0;
}



bool SphereGeometry::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	// TODO: Calculate whether the ray intersects the sphere and fill in the members of the intersection parameter
	return false;
}

void SphereGeometry::getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const {
	// TODO: Fill in the members of the surface parameter	
	// For texCoords: http://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
}

void SphereGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	// TODO: Fill in the members of the surface parameter
	// For texCoords: http://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
	// TIP: Random::sampleUnitSphere gets a random point on a sphere at the origin with a radius of one.
}

BoundingBox SphereGeometry::getBoundingBox() const {
	// TODO: Return a bounding box
	return BoundingBox();
}