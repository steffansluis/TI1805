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
	* Finds the closest object hit by the given ray and returns a pointer to the RayIntersection representing
	* the intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @return A pointer to a RayIntersection representing the intersection point of the ray.
	*/
	std::shared_ptr<const RayIntersection> calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const;

private:
	std::vector<std::shared_ptr<IGeometry>> BTreeAccelerator::retrieveTriangles(const Vec3Df & origin, const Vec3Df & dest) const;

	BTree xTree;
	BTree yTree;
	BTree zTree;
};

#endif