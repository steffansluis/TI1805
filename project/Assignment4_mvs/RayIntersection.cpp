#include "IGeometry.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

void RayIntersection::getSurfacePoint(SurfacePoint &surface) const {
	this->geometry->getSurfacePoint(*this, surface);
}