#ifndef IGEOMETRY_H
#define IGEOMETRY_H

#include "Vec3D.h"

class IMaterial;
class RayIntersection;
class SurfacePoint;

/**
 * Represnts a geometrical object.
 */
class IGeometry {
public:
	virtual ~IGeometry();

	/**
	* Gets the material.
	* @return Pointer to an material.
	*/
	const IMaterial *getMaterial() const;

	/**
	* Sets the material
	* @param[in] material Pointer to an material.
	* @return Pointer to the previous material.
	*/
	const IMaterial *setMaterial(const IMaterial *material);

	/**
	* Perform any necessary preprocessing.
	*/
	virtual void preprocess();

	/**
	 * Calculates the intersection between the given ray and the geometrical object.
	 * Returns null if the ray does not intersect the object.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @return Pointer to a RayIntersection if the ray intersects this object; otherwise null.
	 */
	virtual RayIntersection *calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const = 0;

	/**
	* Gets the surface point on this geometry at the given intersection point.
	* @param[in] intersection An intersection point between a ray and this object.
	* @return The surface point on this geometry at the given intersection point.
	*/
	virtual const SurfacePoint *getSurfacePoint(const RayIntersection *intersection) const = 0;

	/**
	* Gets a random surface point on this geometry.
	* @return A random surface point on this geometry.
	* @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	*/
	virtual const SurfacePoint *getRandomSurfacePoint() const = 0;

private:
	const IMaterial *material;
};

#endif