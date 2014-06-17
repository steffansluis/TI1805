#ifndef IGEOMETRY_H
#define IGEOMETRY_H

#include <memory>

#include "BoundingBox.h"
#include "Vec3D.h"

class IMaterial;
class RayIntersection;
class SurfacePoint;

/**
 * Represents a geometrical object.
 */
class IGeometry : public std::enable_shared_from_this<const IGeometry> {
public:
	IGeometry();
	IGeometry(std::shared_ptr<const IMaterial> material);
	virtual ~IGeometry();

	/**
	* Gets the material.
	* @return Pointer to an material.
	*/
	std::shared_ptr<const IMaterial> getMaterial() const;

	/**
	* Sets the material
	* @param[in] material Pointer to an material.
	*/
	void setMaterial(std::shared_ptr<const IMaterial> material);

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
	virtual std::shared_ptr<const RayIntersection> calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const = 0;

	/**
	* Gets the surface point on this geometry at the given intersection point.
	* @param[in] intersection An intersection point between a ray and this object.
	* @return The surface point on this geometry at the given intersection point.
	*/
	virtual std::shared_ptr<const SurfacePoint> getSurfacePoint(std::shared_ptr<const RayIntersection> intersection) const = 0;

	/**
	* Gets a random surface point on this geometry.
	* @return A random surface point on this geometry.
	* @remarks This will be needed when implementing area lights, so that we can sample the light volume.
	*/
	virtual std::shared_ptr<const SurfacePoint> getRandomSurfacePoint() const = 0;

	/**
	 * Returns a bounding box that bounds this geometry.
	 */
	virtual BoundingBox getBoundingBox() const = 0;

private:
	std::shared_ptr<const IMaterial> material;
};

#endif