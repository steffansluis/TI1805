#ifndef BASETRIANGLEGEOMETRY_H
#define BASETRIANGLEGEOMETRY_H

#include "IGeometry.h"
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
	* Calculates the intersection between the given ray and the triangle.
	* Returns null if the ray does not intersect the triangle.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @return Pointer to a RayIntersection if the ray intersects this triangle; otherwise null.
	*/
	RayIntersection *calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const;

	virtual Vec3Df getVertex0() const = 0;
	virtual Vec3Df getVertex1() const = 0;
	virtual Vec3Df getVertex2() const = 0;

	/**
	* Gets the surface point on this triangle at the given intersection point.
	* @param[in] intersection An intersection point between a ray and this object.
	* @return The surface point on this triangle at the given intersection point.
	*/
	virtual const SurfacePoint *getSurfacePoint(const RayIntersection *intersection) const;

	/**
	* Gets a random surface point on this triangle.
	* @return A random surface point on this triangle.
	*/
	virtual const SurfacePoint *getRandomSurfacePoint() const;

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

	Vec3Df tangent;
	Vec3Df bitangent;
	Vec3Df normal;
};

#endif