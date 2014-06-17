#ifndef IACCELERATIONSTRUCTURE_H
#define IACCELERATIONSTRUCTURE_H

#include <memory>
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
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> getGeometry() const;

	/**
	 * Sets the vector containing all geometry in this structure.
	 * @param[in] accelerator Pointer to vector containing all the geometry in this structure
	 * @return Pointer to the old geometry vector.
	 */
	void setGeometry(std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry);

	/**
	 * Perform any necessary preprocessing.
	 */
	virtual void preprocess();

	/*
	 * Returns whether any object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the closest point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	virtual bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const = 0;

	/*
	 * Returns whether any object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param maxDistance The maximum distance at which the intersection may occur.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	virtual bool calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const = 0;

private:
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry;
};

#endif