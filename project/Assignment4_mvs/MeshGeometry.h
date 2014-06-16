#ifndef MESHGEOMETRY_H
#define MESHGEOMETRY_H

#include <vector>

#include "IGeometry.h"
#include "Vec3D.h"

class IAccelerationStructure;
class Mesh;

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
	* Gets the acceleration structure that is used to find speed up
	* the intersection calculations.
	* @return Pointer to an acceleration structure.
	*/
	std::shared_ptr<IAccelerationStructure> getAccelerationStructure() const;

	/**
	* Sets the acceleration structure that is used to find speed up
	* the intersection calculations.
	* @param[in] accelerator Pointer to an acceleration structure.
	*/
	void setAccelerationStructure(std::shared_ptr<IAccelerationStructure> accelerator);

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
	std::shared_ptr<const RayIntersection> calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const;

	/**
	* Gets the surface point on this mesh at the given intersection point.
	* @param[in] intersection An intersection point between a ray and this object.
	* @return The surface point on this mesh at the given intersection point.
	*/
	std::shared_ptr<const SurfacePoint> getSurfacePoint(std::shared_ptr<const RayIntersection> intersection) const;

	/**
	* Gets a random surface point on this mesh.
	* @return A random surface point on this mesh.
	*/
	std::shared_ptr<const SurfacePoint> getRandomSurfacePoint() const;

private:
	static std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> generateTriangles(const Mesh *mesh);

	const Mesh *mesh;
	std::shared_ptr<IAccelerationStructure> accelerator;
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> triangles;
};

#endif