#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vec3D.h"

class BoundingBox {
public:
	/**
	 * Initializes a bounding box to be empty.
	 */
	BoundingBox();

	/**
	 * Initializes a bounding box defined by the given minimum and maximum.
	 * @param[in] min The minimum point contained by the bounding box.
	 * @param[in] max The maximum point contained by the bounding box.
	 */
	BoundingBox(const Vec3Df &min, const Vec3Df &max);

	/**
	 * Tests whether the given ray intersects the bounding box.
	 * @param[in] origin The origin of the vector
	 * @param[in] dir The direction of the vector
	 * @return Returns true if the ray intersects the bounding box; otherwise false.
	 */
	bool intersects(const Vec3Df &origin, const Vec3Df &dir) const;

	/**
	 * Tests whether the given ray intersects the bounding box and calculates
	 * the distance at which the intersection occurred.
	 * @param[in] origin The origin of the vector
	 * @param[in] dir The direction of the vector
	 * @param[out] distance The distance at which the intersection occurred, if any.
	 * @return Returns true if the ray intersects the bounding box; otherwise false.
	 */
	bool intersects(const Vec3Df &origin, const Vec3Df &dir, float &distance) const;

	/**
	 * Tests whether this bounding box intersects with the given bounding box.
	 * @param[in] A bounding box.
	 * @return True if the bounding boxes intersect; otherwise false.
	 */
	bool intersects(const BoundingBox &other) const;

	/**
	 * Computes the center of the bounding box.
	 * @return The center of the bounding box.
	 */
	Vec3Df getCenter() const;

	/**
	 * Grows the bounding box if needed so that it includes the given point.
	 * @param[in] point The point to be occluded in the bounding box.
	 */
	void includePoint(const Vec3Df &point);

	/**
	 * The minimum point contained by the bounding box.
	 */
	Vec3Df min;

	/**
	 * The maximum point contained by the bounding box.
	 */
	Vec3Df max;
};

#endif