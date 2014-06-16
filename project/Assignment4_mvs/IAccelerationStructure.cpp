#include <assert.h>

#include "IAccelerationStructure.h"
#include "IGeometry.h"
#include "RayIntersection.h"

IAccelerationStructure::~IAccelerationStructure() {
}

void IAccelerationStructure::preprocess() {
}

const std::vector<IGeometry*> *IAccelerationStructure::getGeometry() const {
	return this->geometry;
}

const std::vector<IGeometry*> *IAccelerationStructure::setGeometry(const std::vector<IGeometry*> *geometry) {
	assert(geometry);

	const std::vector<IGeometry*> *oldGeometry = this->geometry;

	this->geometry = geometry;

	return oldGeometry;
}