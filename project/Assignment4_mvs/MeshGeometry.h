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
	* Gets the surface area of the geometry.
	* @return The surface area of the geometry.
	*/
	float getArea() const;

	/*
	 * Calculates whether the mesh is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the closest point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/*
	 * Returns whether the mesh is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param maxDistance The maximum distance at which the intersection may occur.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	bool calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const;

	/**
	 * Gets the surface point on this mesh at the given intersection point.
	 * @param[in] intersection An intersection point between a ray and this object.
	 * @return The surface point on this mesh at the given intersection point.
	 */
	void getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const;

	/**
	 * Gets a random surface point on this mesh.
	 * @return A random surface point on this mesh.
	 */
	void getRandomSurfacePoint(SurfacePoint &surface) const;

	BoundingBox getBoundingBox() const;

private:
	static std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> generateTriangles(const Mesh *mesh);

	static BoundingBox createBoundingBox(const Mesh *mesh);

	const Mesh *mesh;
	float totalArea;
	float maxTriangleArea;
	BoundingBox boundingBox;
	std::shared_ptr<IAccelerationStructure> accelerator;
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> triangles;
};

#endif