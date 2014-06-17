#include <cassert>

#include "DiffuseMaterial.h"
#include "IGeometry.h"
#include "IMaterial.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

static auto diffuseMaterial = std::make_shared<const DiffuseMaterial>();

IGeometry::IGeometry()
: IGeometry(diffuseMaterial) {
}

IGeometry::IGeometry(std::shared_ptr<const IMaterial> material) {
	this->material = material;
}

IGeometry::~IGeometry() {
}

std::shared_ptr<const IMaterial> IGeometry::getMaterial() const {
	return this->material;
}

void IGeometry::setMaterial(std::shared_ptr<const IMaterial> material) {
	if (material)
		this->material = material;
	else
		this->material = diffuseMaterial;
}

void IGeometry::preprocess() {
}