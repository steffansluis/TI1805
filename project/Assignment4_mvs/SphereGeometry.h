#ifndef SPHEREGEOMETRY_H
#define SPHEREGEOMETRY_H

#include "IGeometry.h"

class SphereGeometry : public IGeometry {
public:
	/**
	 * Initializes a new sphere with the given position and radius.
	 */
	SphereGeometry(const Vec3Df &position, float radius);

	/**
	* Gets the surface area of the geometry.
	* @return The surface area of the geometry.
	*/
	float getArea() const;

	/*
	 * Calculates whether the sphere is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the closest point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/**
	 * Gets the surface point on this geometry at the given intersection point.
	 * @param[in] intersection An intersection point between a ray and this object.
	 * @return The surface point on this geometry at the given intersection point.
	 */
	void getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const;

	/**
	 * Gets a random surface point on this sphere.
	 * @return A random surface point on this sphere.
	 * @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	 */
	void getRandomSurfacePoint(SurfacePoint &surface) const;

	/**
	 * Returns a bounding box that bounds this geometry.
	 */
	BoundingBox getBoundingBox() const;

private:
	Vec3Df position;
	float radius;
};

#endif