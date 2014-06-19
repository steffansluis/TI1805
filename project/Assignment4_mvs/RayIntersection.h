#ifndef RAYINTERSECTION_H
#define RAYINTERSECTION_H

#include <memory>

#include "Vec3D.h"

class IGeometry;
class SurfacePoint;

/**
 * Represents the intersection point between a ray and scene geometry.
 */
class RayIntersection
{
public:
	/**
	 * Gets the surface point of the geometry at the point of intersection.
	 * @param[out] surface The surface point of the geometry at the point of intersection.
	 */
	void getSurfacePoint(SurfacePoint &surface) const;

	/**
	 * The point where the geometry was intersected.
	 */
	Vec3Df hitPoint;


	/**
	 * The origin of the intersecting ray.
	 *
	 * This is the original data for the ray. 
	 * The equation for the ray is as follows:
	 * R(t) = origin + distance * direction;
	 */
	Vec3Df origin;
	
	/**
	 * The direction of the intersecting ray.
	 *
	 * This is the original data for the ray. 
	 * The equation for the ray is as follows:
	 * R(t) = origin + distance * direction;
	 */
	Vec3Df direction;

	/**
	 * The distance along the ray at which the intersection occured.
	 *
	 * This is the original data for the ray. 
	 * The equation for the ray is as follows:
	 * R(t) = origin + distance * direction;
	 */
	float distance;

	/**
	 * The geometry that the ray intersects with.
	 */
	std::shared_ptr<const IGeometry> geometry;
};


#endif