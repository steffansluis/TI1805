#include "IGeometry.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

std::shared_ptr<const SurfacePoint> RayIntersection::getSurfacePoint() const {
	return this->geometry->getSurfacePoint(this->shared_from_this());
}