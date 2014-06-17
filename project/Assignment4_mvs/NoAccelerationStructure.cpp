#include "IGeometry.h"
#include <limits>

#include "NoAccelerationStructure.h"
#include "RayIntersection.h"

bool NoAccelerationStructure::calculateClosestIntersection(const Vec3Df & origin, const Vec3Df & dir, RayIntersection &intersection) const {
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

	RayIntersection lastIntersection;	
	intersection = RayIntersection();
	intersection.distance = std::numeric_limits<float>::infinity();

	bool intersectsAny = false;

	// Iterate through all geometry in the scene
	for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = geometry->begin(); it != geometry->end(); ++it) {
		// Find the intersection between the ray and the geometry
		bool intersects = (*it)->calculateClosestIntersection(origin, dir, lastIntersection);

		// If no intersection was found, continue
		if (!intersects) {
			continue;
		}

		// If this is the first intersection, set it intersection the the last intersection
		if (!intersectsAny) {
			intersection = lastIntersection;
			intersectsAny = true;
		}
		// Otherwise, set intersection to last intersection only if it is closer than the current best intersection
		else if (lastIntersection.distance < intersection.distance) {
			intersection = lastIntersection;
		}
	}

	return intersectsAny;
}

bool NoAccelerationStructure::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

	// Iterate through all geometry in the scene
	for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = geometry->begin(); it != geometry->end(); ++it) {
		// Find the intersection between the ray and the geometry
		bool intersects = (*it)->calculateAnyIntersection(origin, dir, maxDistance, intersection);

		// If an intersection was found, return it
		if (intersects) {
			return true;
		}
	}

	return false;
}