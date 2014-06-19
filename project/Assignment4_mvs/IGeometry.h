#ifndef IGEOMETRY_H
#define IGEOMETRY_H

#include <memory>

#include "BoundingBox.h"
#include "Vec3D.h"

class IMaterial;
class RayIntersection;
class SurfacePoint;

/**
 * Represents a geometrical object.
 */
class IGeometry : public std::enable_shared_from_this<const IGeometry> {
public:
	IGeometry();
	IGeometry(std::shared_ptr<const IMaterial> material);
	virtual ~IGeometry();

	/**
	 * Gets the material.
	 * @return Pointer to an material.
	 */
	std::shared_ptr<const IMaterial> getMaterial() const;

	/**
	 * Sets the material
	 * @param[in] material Pointer to an material.
	 */
	void setMaterial(std::shared_ptr<const IMaterial> material);

	/**
	 * Perform any necessary preprocessing.
	 */
	virtual void preprocess();

	/*
	 * Calculates whether the object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the closest point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	virtual bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const = 0;

	/*
	 * Returns whether the object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param maxDistance The maximum distance at which the intersection may occur.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	virtual bool calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const;

	/**
	 * Gets the surface point on this geometry at the given intersection point.
	 * @param[in] intersection An intersection point between a ray and this object.
	 * @return The surface point on this geometry at the given intersection point.
	 */
	virtual void getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const = 0;

	/**
	 * Gets a random surface point on this geometry.
	 * @return A random surface point on this geometry.
	 * @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	 */
	virtual void getRandomSurfacePoint(SurfacePoint &surface) const = 0;

	/**
	 * Returns a bounding box that bounds this geometry.
	 */
	virtual BoundingBox getBoundingBox() const = 0;

private:
	std::shared_ptr<const IMaterial> material;
};

#endif