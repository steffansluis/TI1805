#ifndef BASETRIANGLEGEOMETRY_H
#define BASETRIANGLEGEOMETRY_H

#include "IGeometry.h"
#include "Vec2D.h"
#include "Vec3D.h"

/**
 * Represents a triangle.
 * The vertices are abstract so that we can also wrap an indexed triangle in a mesh.
 */
class BaseTriangleGeometry : public IGeometry {
public:
	/**
	 * Perform any necessary preprocessing.
	 */
	void preprocess();

	/**
	* Gets the surface area of the geometry.
	* @return The surface area of the geometry.
	*/
	float getArea() const;

	/*
	 * Returns whether any object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the closest point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	virtual Vec3Df getVertex0() const = 0;
	virtual Vec3Df getVertex1() const = 0;
	virtual Vec3Df getVertex2() const = 0;

	/**
	 * Gets the surface point on this triangle at the given intersection point.
	 * @param[in] intersection An intersection point between a ray and this object.
	 * @return The surface point on this triangle at the given intersection point.
	 */
	virtual void getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const;

	/**
	 * Gets a random surface point on this triangle.
	 * @return A random surface point on this triangle.
	 */
	virtual void getRandomSurfacePoint(SurfacePoint &surface) const;

	BoundingBox getBoundingBox() const;

private:
	/**
	 * Calculates the whether the given point lies in this triangle.
	 * @param[in] point A point.
	 * @param[in] normal The surface normal.
	 * @param[in] v0 The first vertex.
	 * @param[in] v1 The second vertex.
	 * @param[in] v2 The third vertex.
	 * @return True if the given point lies in this triangle; otherwise false.
	 */
	static bool calculateRayInsideTriangle(const Vec3Df &point, const Vec3Df &normal, const Vec3Df &v0, const Vec3Df &v1, const Vec3Df &v2);

	/**
	 * Calculates the barycentric coordinates of the given point.
	 * @param point The point of which to compute the barycentric coordinates.
	 * @return The uv-components of the barycentric coordinates. The w-component can be computed as 1 - u - v.
	 * @remarks This is method is very fast but inaccurate, do not use for testing intersections.
	 */
	Vec2Df calculateBarycentricCoordinates(const Vec3Df &point) const;

	/**
	 * Calculates a random point on the triangle.
	 * @return A random point on the triangle.
	 */
	Vec3Df calculateRandomPoint() const;

	float area;
	Vec3Df tangent;
	Vec3Df bitangent;
	Vec3Df normal;
};

#endif