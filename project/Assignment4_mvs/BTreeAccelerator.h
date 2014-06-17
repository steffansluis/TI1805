#ifndef BTREEACCELERATOR_H
#define BTREEACCELERATOR_H

#include "BTree.h"
#include "IAccelerationStructure.h"

class RayIntersection;

class BTreeAccelerator : public IAccelerationStructure {
public:
	BTreeAccelerator();

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
	std::vector<std::shared_ptr<IGeometry>> BTreeAccelerator::retrieveTriangles(const Vec3Df & origin, const Vec3Df & dest) const;

	BTree xTree;
	BTree yTree;
	BTree zTree;
};

#endif