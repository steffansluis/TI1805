#ifndef OCTREEACCELERATOR_H
#define OCTREEACCELERATOR_H

#include "IAccelerationStructure.h"
#include "OctreeNode.h"

class RayIntersection;

class Octree : public IAccelerationStructure {
public:
	Octree();

	/**
	* Perform any necessary preprocessing.
	*/
	void preprocess();

	/*
	* Returns whether any object is hit by the given ray and sets the intersection parameter
	* to the RayIntersection representing the closest point of intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	* @return True if the ray intersected an object; otherwise false.
	*/
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/*
	* Returns whether any object is hit by the given ray and sets the intersection parameter
	* to the RayIntersection representing the point of intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param maxDistance The maximum distance at which the intersection may occur.
	* @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	* @return True if the ray intersected an object; otherwise false.
	*/
	bool calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const;

private:
	OctreeNode *root;
};

#endif