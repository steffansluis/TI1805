#ifndef PLANEGEOMETRY_H
#define PLANEGEOMETRY_H

#include "IGeometry.h"
#include "Vec3D.h"

class PlaneGeometry : public IGeometry {
public:
	/**
	* Initializes a new plane with the given normal and distance from origin.
	* @param[in] normal The normal of the plane.
	* @param distance The distance from the origin in the direction of the normal.
	*/
	PlaneGeometry(const Vec3Df &normal, float distance);
	/**
	* Initializes a new plane with the given normal and point.
	* @param[in] normal The normal of the plane.
	* @param[in] normal A point on the plane.
	*/
	PlaneGeometry(const Vec3Df &normal, const Vec3Df &point);

	/**
	* Gets the surface area of the geometry.
	* @return The surface area of the geometry.
	*/
	virtual float getArea() const;

	/*
	* Calculates whether the object is hit by the given ray and sets the intersection parameter
	* to the RayIntersection representing the closest point of intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	* @return True if the ray intersected an object; otherwise false.
	*/
	virtual bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/**
	 * Gets the surface point on this geometry at the given intersection point.
	 * @param[in] intersection An intersection point between a ray and this object.
	 * @return The surface point on this geometry at the given intersection point.
	 */
	void getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const;

	/**
	 * Gets a random surface point on this geometry.
	 * @return A random surface point on this geometry.
	 * @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	 */
	virtual void getRandomSurfacePoint(SurfacePoint &surface) const;

	/**
	 * Returns a bounding box that bounds this geometry.
	 */
	virtual BoundingBox getBoundingBox() const;

protected:
	Vec3Df normal;
	float distance;
};

#endif