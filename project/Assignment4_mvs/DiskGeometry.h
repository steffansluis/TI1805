#ifndef DISKGEOMETRY_H
#define DISKGEOMETRY_H

#include "PlaneGeometry.h"

class DiskGeometry : public PlaneGeometry {
public:
	/**
	* Initializes a new disk with the given normal, point and radius.
	* @param[in] normal The normal of the disk.
	* @param[in] center The center of the disk.
	* @param radius The radius of the disk.
	*/
	DiskGeometry(const Vec3Df &normal, const Vec3Df &center, float radius);

	/**
	* Gets the surface area of the geometry.
	* @return The surface area of the geometry.
	*/
	float getArea() const;

	/*
	* Calculates whether the object is hit by the given ray and sets the intersection parameter
	* to the RayIntersection representing the closest point of intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	* @return True if the ray intersected an object; otherwise false.
	*/
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/**
	* Gets a random surface point on this geometry.
	* @return A random surface point on this geometry.
	* @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	*/
	void getRandomSurfacePoint(SurfacePoint &surface) const;

	/**
	* Returns a bounding box that bounds this geometry.
	*/
	BoundingBox getBoundingBox() const;

private:
	Vec3Df center;
	float radius;
};

#endif