#include "RayIntersection.h"
#include "SurfacePoint.h"

const SurfacePoint *RayIntersection::getSurfacePoint() const {
	return this->geometry->getSurfacePoint(this);
}