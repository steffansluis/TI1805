#ifndef MESHTRIANGLEGEOMETRY_H
#define MESHTRIANGLEGEOMETRY_H

#include "BaseTriangleGeometry.h"
#include "Vec2D.h"
#include "Vec3D.h"

class Mesh;
class RayIntersection;
class SurfacePoint;
class Triangle;

/**
* Represents a triangle inside a mesh.
*/
class MeshTriangleGeometry : public BaseTriangleGeometry {
public:
	/**
	 * Initializes a MeshTriangleGeometry with the given mesh and triangle.
	 * @param[in] mesh The mesh that this triangle belongs to.
	 * @param[in] triangle The triangle that this TriangleGeometry wraps.
	 */
	MeshTriangleGeometry(const Mesh *mesh, const Triangle *triangle);

	Vec3Df getVertex0() const;
	Vec3Df getVertex1() const;
	Vec3Df getVertex2() const;

	/**
	* Gets the surface point on this triangle at the given intersection point.
	* @param[in] intersection An intersection point between a ray and this object.
	* @return The surface point on this triangle at the given intersection point.
	*/
	void getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const;

	/**
	* Gets a random surface point on this triangle.
	* @return A random surface point on this triangle.
	* @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	*/
	void getRandomSurfacePoint(SurfacePoint &surface) const;

private:
	/**
	 * Calculates the surface normal at the given barycentric coordinates
	 * by interpolating the triangle's vertex normals.
	 * @param[in] uv The barycentric coordinates.
	 * @return The surface normal at the given barycentric coordinates.
	 */
	Vec3Df getSurfaceNormal(const Vec2Df &uv) const;

	/**
	 * Calculates the texture coordinates at the given barycentric coordinates
	 * by interpolating the triangle's texture coordinates.
	 * @param[in] uv The barycentric coordinates.
	 * @param[out] tUv The uv texture coordinate.
	 */
	void getTextureCoordinates(const Vec2Df &uv, Vec2Df &tUV) const;

	const Mesh *mesh;
	const Triangle *triangle;
};

#endif