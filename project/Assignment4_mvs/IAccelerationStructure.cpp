#include <assert.h>

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