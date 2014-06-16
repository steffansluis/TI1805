#ifndef RAYINTERSECTION_H
#define RAYINTERSECTION_H

#include <memory>

#include "Vec3D.h"

class IGeometry;
class SurfacePoint;

/**
 * Represents the intersection point between a ray and scene geometry.
 */
class RayIntersection : public std::enable_shared_from_this<const RayIntersection>
{
public:
	/**
	 * Gets the surface point of the geometry at the point of intersection.
	 * @return The surface point of the geometry at the point of intersection.
	 */
	std::shared_ptr<const SurfacePoint> getSurfacePoint() const;

	/**
	* The point where the geometry was intersected.
	*/
	Vec3Df hitPoint;

	/**
	* The surface normal at the point of intersection.
	*/
	Vec3Df normal;

	// This is the original data for the ray. 
	// The equation for the ray is as follows:
	// R(t) = origin + distance * direction;
	// This equation is the exact same as the hitPoint.

	/**
	* The origin of the intersecting ray.
	*/
	Vec3Df origin;

	/**
	* The direction of the intersecting ray.
	*/
	Vec3Df direction;

	/**
	* The distance along the ray at which the intersection occured.
	*/
	float distance;

	/**
	* The geometry that the ray intersects with.
	*/
	std::shared_ptr<const IGeometry> geometry;
};


#endif