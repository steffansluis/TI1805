#include <assert.h>

#include "IGeometry.h"
#include "IMaterial.h"

IGeometry::~IGeometry() {
}

const IMaterial *IGeometry::getMaterial() const {
	return this->material;
}

const IMaterial *IGeometry::setMaterial(const IMaterial *material) {
	assert(material);

	const IMaterial *oldMaterial = this->material;

	this->material = material;

	return oldMaterial;
}

void IGeometry::preprocess() {
}