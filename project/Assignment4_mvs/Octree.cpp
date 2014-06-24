#include <limits>

#include "Octree.h"
#include "RayIntersection.h"

Octree::Octree() {
}

void Octree::preprocess() {
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

	std::vector<std::shared_ptr<IGeometry>> *tempGeometry = new std::vector<std::shared_ptr<IGeometry>>(geometry->begin(), geometry->end());

	this->root = new OctreeNode(tempGeometry);
}

bool Octree::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	intersection = RayIntersection();
	intersection.distance = std::numeric_limits<float>::infinity();

	return this->root->calculateClosestIntersection(origin, dir, intersection);
}

bool Octree::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	return this->root->calculateAnyIntersection(origin, dir, maxDistance, intersection);
}
