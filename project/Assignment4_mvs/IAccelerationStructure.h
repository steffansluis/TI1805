#ifndef IACCELERATIONSTRUCTURE_H
#define IACCELERATIONSTRUCTURE_H

#include <vector>

#include "Vec3D.h"

class IGeometry;
class RayIntersection;

class IAccelerationStructure {
public:
	virtual ~IAccelerationStructure();

	/**
	* Gets the vector containing all geometry in this structure.
	* @return Pointer to a vector containing all the geometry in this structure.
	*/
	const std::vector<IGeometry*> *getGeometry() const;

	/**
	* Sets the vector containing all geometry in this structure.
	* @param[in] accelerator Pointer to vector containing all the geometry in this structure
	* @return Pointer to the old geometry vector.
	*/
	const std::vector<IGeometry*> *setGeometry(const std::vector<IGeometry*> *geometry);

	/**
	* Perform any necessary preprocessing.
	*/
	virtual void preprocess();

	/*
	 * Finds the closest object hit by the given ray and returns a pointer to the RayIntersection representing
	 * the intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @return A pointer to a RayIntersection representing the intersection point of the ray.
	 */
	virtual RayIntersection *calculateIntersection(const Vec3Df & origin, const Vec3Df & dir) const = 0;
	
	// TODO: calculateIntersection for shadow rays

private:
	const std::vector<IGeometry*> *geometry;
};

#endif