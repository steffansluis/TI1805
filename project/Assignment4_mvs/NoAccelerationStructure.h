#ifndef NOACCELERATIONSTRUCTURE_H
#define NOACCELERATIONSTRUCTURE_H

#include "IAccelerationStructure.h"
#include "RayIntersection.h"
#include "Scene.h"

/**
 * Implements an acceleration structure that simply tests every
 * object indiscriminately, providing no speed up.
 */
class NoAccelerationStructure : public IAccelerationStructure {
public:
	/*
	* Finds the closest object hit by the given ray and returns a pointer to the RayIntersection representing
	* the intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @return A pointer to a RayIntersection representing the intersection point of the ray.
	*/
	RayIntersection *calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const;
};

#endif