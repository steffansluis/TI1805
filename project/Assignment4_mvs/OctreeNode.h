#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <vector>

#include "BoundingBox.h"
#include "IGeometry.h"

class OctreeNode {
public:
	OctreeNode();
	OctreeNode(const std::vector<std::shared_ptr<IGeometry>> *geometry);
	OctreeNode(const BoundingBox &innerBox, const std::vector<std::shared_ptr<IGeometry>> *geometry);
	~OctreeNode();

	/*
	* Returns whether any object is hit by the given ray and sets the intersection parameter
	* to the RayIntersection representing the closest point of intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	* @return True if the ray intersected an object; otherwise false.
	*/
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/*
	* Returns whether any object is hit by the given ray and sets the intersection parameter
	* to the RayIntersection representing the point of intersection.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param maxDistance The maximum distance at which the intersection may occur.
	* @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	* @return True if the ray intersected an object; otherwise false.
	*/
	bool calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const;

private:
	/**
	* Splits the given bounding box into 8 children.
	*/
	static void createChildBoundingBoxes(const BoundingBox &box, BoundingBox children[8]);

	/**
	 * Creates a box bounding the given geometry.
	 */
	static BoundingBox createBoundingBox(const std::vector<std::shared_ptr<IGeometry>> *geometry);

	/**
	 *
	 * @param[in] boundingBox
	 * @param[in] geometry
	 * @param[out] children
	 * @return
	 */
	static bool subdivide(const BoundingBox &boundingBox, const std::vector<std::shared_ptr<IGeometry>> *geometry, OctreeNode *children[8]);

	BoundingBox boundingBox;
	OctreeNode *children[8];
	const std::vector<std::shared_ptr<IGeometry>> *geometry;
};

#endif