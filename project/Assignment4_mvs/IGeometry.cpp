#include <cassert>

#include "DiffuseMaterial.h"
#include "IGeometry.h"
#include "IMaterial.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

IGeometry::IGeometry()
: IGeometry(std::make_shared<const DiffuseMaterial>()) {
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
	assert(material);

	this->material = material;
}

void IGeometry::preprocess() {
}

bool IGeometry::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	// If no intersection was found or the closest intersection is beyond the maximum distance, return false; otherwise true.
	if (!this->calculateClosestIntersection(origin, dir, intersection) || intersection.distance > maxDistance)
		return false;
	else
		return true;
}