#ifndef PLANEGEOMETRY_H
#define PLANEGEOMETRY_H

#include "IGeometry.h"
#include "Vec3D.h"

class PlaneGeometry : public IGeometry {
public:
	/**
	 * Initializes a new plane with the given normal and distance from origin.
	 * @param[in] normal The normal of the plane.\*$^([ \t])
	 * @param distance The distance from the origin in the direction of the normal.
	 */
	PlaneGeometry(const Vec3Df &normal, float distance);

	/**
	* Gets the surface area of the geometry.
	* @return The surface area of the geometry.
	*/
	float getArea() const;

	/**
	 * Returns whether the object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the point of intersection.
	 *
	 * @param[in]	origin		The origin of the ray.
	 * @param[in]	dir			The direction of the ray.
	 * @param[in]	maxDistance	The maximum distance at which the intersection may occur.
	 * @param[out]				intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return					True if the ray intersected an object; otherwise false.
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
	Vec3Df normal;
	float distance;
};

#endif