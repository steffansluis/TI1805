#include <assert.h>
#include <limits>

#include "IAccelerationStructure.h"
#include "IGeometry.h"
#include "RayIntersection.h"

IAccelerationStructure::~IAccelerationStructure() {
}

void IAccelerationStructure::preprocess() {
}

std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> IAccelerationStructure::getGeometry() const {
	return this->geometry;
}

void IAccelerationStructure::setGeometry(std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry) {
	this->geometry = geometry;
}

bool IAccelerationStructure::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	// By default just use the regular intersection method
	return this->calculateClosestIntersection(origin, dir, intersection);
}