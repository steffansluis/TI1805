#include "NoAccelerationStructure.h"

RayIntersection *NoAccelerationStructure::calculateIntersection(const Vec3Df & origin, const Vec3Df & dir) const {
	const std::vector<IGeometry*> *geometry = this->getGeometry();

	RayIntersection *bestIntersection = NULL;

	// Iterate through all geometry in the scene
	for (std::vector<IGeometry*>::const_iterator it = geometry->begin(); it != geometry->end(); ++it) {
		// Find the intersection between the ray and the geometry
		RayIntersection *intersection = (*it)->calculateIntersection(origin, dir);

		// If no intersection was found, continue
		if (intersection == NULL) {
			continue;
		}

		// If this is the first intersection, assign it to best intersection
		if (bestIntersection == NULL) {
			bestIntersection = intersection;
		}
		// Otherwise, assign intersection to best intersection only if it is closer.
		// Make sure to clean up the previous best intersection.
		else if (intersection->distance < bestIntersection->distance) {
			delete bestIntersection;

			bestIntersection = intersection;
		}
	}

	// Return the best (e.g. closest) intersection.
	return bestIntersection;
}