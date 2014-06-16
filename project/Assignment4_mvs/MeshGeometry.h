#ifndef MESHGEOMETRY_H
#define MESHGEOMETRY_H

#include <vector>

#include "IGeometry.h"
#include "Vec3D.h"

class IAccelerationStructure;
class IGeometry;
class Mesh;
class SurfacePoint;

/**
 * Represents a triangle mesh.
 */
class MeshGeometry : public IGeometry {
public:
	/**
	 * Initializes a MeshGeometry with the given mesh.
	 */
	MeshGeometry(const Mesh *mesh);
	~MeshGeometry();

	/**
	 * Returns the underlaying mesh that this object wraps.
	 * @return The underlaying mesh that this object wraps.
	 */
	const Mesh *getMesh() const;

	/**
	* Gets the acceleration structure that is used to find speed up
	* the intersection calculations.
	* @return Pointer to an acceleration structure.
	*/
	IAccelerationStructure *getAccelerationStructure() const;

	/**
	* Sets the acceleration structure that is used to find speed up
	* the intersection calculations.
	* @param[in] accelerator Pointer to an acceleration structure.
	* @return Pointer to previous acceleration structure.
	*/
	IAccelerationStructure *setAccelerationStructure(IAccelerationStructure *accelerator);

	/**
	* Perform any necessary preprocessing.
	*/
	void preprocess();

	/**
	* Calculates the intersection between the given ray and the mesh.
	* Returns null if the ray does not intersect the mesh.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @return Pointer to a RayIntersection if the ray intersects this mesh; otherwise null.
	*/
	RayIntersection *calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const;

	/**
	* Gets the surface point on this mesh at the given intersection point.
	* @param[in] intersection An intersection point between a ray and this object.
	* @return The surface point on this mesh at the given intersection point.
	*/
	const SurfacePoint *getSurfacePoint(const RayIntersection *intersection) const;

	/**
	* Gets a random surface point on this mesh.
	* @return A random surface point on this mesh.
	*/
	const SurfacePoint *getRandomSurfacePoint() const;

private:
	const Mesh *mesh;
	IAccelerationStructure *accelerator;
	std::vector<IGeometry *> triangles;
};

#endif