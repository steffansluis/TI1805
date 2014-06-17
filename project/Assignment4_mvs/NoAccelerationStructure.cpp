#include "IGeometry.h"
#include "NoAccelerationStructure.h"
#include "RayIntersection.h"

std::shared_ptr<const RayIntersection> NoAccelerationStructure::calculateIntersection(const Vec3Df & origin, const Vec3Df & dir) const {
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

	std::shared_ptr<const RayIntersection> bestIntersection = nullptr;

	// Iterate through all geometry in the scene
	for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = geometry->begin(); it != geometry->end(); ++it) {
		// Find the intersection between the ray and the geometry
		std::shared_ptr<const RayIntersection> intersection = (*it)->calculateIntersection(origin, dir);

		// If no intersection was found, continue
		if (intersection == nullptr) {
			continue;
		}

		// If this is the first intersection, assign it to best intersection
		if (bestIntersection == nullptr) {
			bestIntersection = intersection;
		}
		// Otherwise, assign intersection to best intersection only if it is closer.
		else if (intersection->distance < bestIntersection->distance) {
			bestIntersection = intersection;
		}
	}

	// Return the best (e.g. closest) intersection.
	return bestIntersection;
}