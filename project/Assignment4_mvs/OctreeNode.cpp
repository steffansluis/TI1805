#include <algorithm>

#include "BoundingBox.h"
#include "IGeometry.h"
#include "OctreeNode.h"
#include "RayIntersection.h"

OctreeNode::OctreeNode() 
: geometry(nullptr) {
}

OctreeNode::OctreeNode(const std::vector<std::shared_ptr<IGeometry>> *geometry) 
: OctreeNode(OctreeNode::createBoundingBox(geometry), geometry)
{
}

OctreeNode::OctreeNode(const BoundingBox &innerBox, const std::vector<std::shared_ptr<IGeometry>> *geometry) {
	this->boundingBox = OctreeNode::createBoundingBox(geometry);
	
	if (geometry->size() > 32 && subdivide(innerBox, geometry, this->children)) {
		this->geometry = nullptr;

		delete geometry;
	}
	else {
		this->geometry = geometry;
	}
}

OctreeNode::~OctreeNode() {
	if (this->geometry) {
		delete this->geometry;
		this->geometry = reinterpret_cast<const std::vector<std::shared_ptr<IGeometry>>*>(0xDEADBEEF);
	}
	else {
		for (int i = 0; i < 8; i++) {
			delete this->children[i];
			this->children[i] = reinterpret_cast<OctreeNode*>(0xDEADBEEF);
		}
	}
}

bool OctreeNode::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	RayIntersection lastIntersection;
	float distance;

	// Intersect against the bounding box
	if (!this->boundingBox.intersects(origin, dir, distance) || distance > intersection.distance)
		return false;

	bool intersectsAny = false;

	if (this->geometry) {
		for (unsigned int i = 0; i < this->geometry->size(); i++) {
			bool intersects = this->geometry->at(i)->calculateClosestIntersection(origin, dir, lastIntersection);

			// If no intersection was found, continue
			if (!intersects) {
				continue;
			}

			// If this is the first intersection, set it intersection the the last intersection
			if (!intersectsAny) {
				intersectsAny = true;
			}

			// Set intersection to last intersection only if it is closer than the current best intersection
			if (lastIntersection.distance < intersection.distance) {
				intersection = lastIntersection;
			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			bool intersects = this->children[i]->calculateClosestIntersection(origin, dir, intersection);

			// If this is the first intersection, set it intersection the the last intersection
			if (!intersects) {
				intersectsAny = true;
			}
		}
	}

	return intersectsAny;
}

bool OctreeNode::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	float distance;

	// Intersect against the bounding box
	if (!this->boundingBox.intersects(origin, dir, distance) || distance > maxDistance)
		return false;

	bool intersectsAny = false;

	if (this->geometry) {
		for (unsigned int i = 0; i < this->geometry->size(); i++) {
			// Find the intersection between the ray and the geometry
			bool intersects = this->geometry->at(i)->calculateAnyIntersection(origin, dir, maxDistance, intersection);

			// If an intersection was found, return it
			if (intersects) {
				return true;
			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			// Find the intersection between the ray and the geometry
			bool intersects = this->children[i]->calculateAnyIntersection(origin, dir, maxDistance, intersection);

			// If an intersection was found, return it
			if (intersects) {
				return true;
			}
		}
	}

	return false;
}

bool OctreeNode::subdivide(const BoundingBox &boundingBox, const std::vector<std::shared_ptr<IGeometry>> *geometry, OctreeNode *children[8]) {
	BoundingBox childBoxes[8];
	std::vector<std::shared_ptr<IGeometry>> *childData[8];

	// Compute the bounding boxes of the children
	OctreeNode::createChildBoundingBoxes(boundingBox, childBoxes);

	unsigned int biggestChild = 0;
	unsigned int totalIntersectionTests = 8;

	// Create an array of intersecting geometry for each bounding box and count the total number of
	// intersection tests required for the given geometry set after subdividing.
	for (int i = 0; i < 8; i++) {
		childData[i] = new std::vector<std::shared_ptr<IGeometry>>();
		unsigned int child = 0;

		for (unsigned int j = 0; j < geometry->size(); j++) {
			if (childBoxes[i].intersects(geometry->at(j)->getBoundingBox())) {
				childData[i]->push_back(geometry->at(j));

				totalIntersectionTests++;
				child++;
			}
		}

		biggestChild = std::max(biggestChild, child);
	}

	// Some arbitary heuristic, if the total number of intersection tests increases by more
	// than a factor of two, do not subdivide.
	if ((biggestChild > geometry->size() / 4) || (totalIntersectionTests > 2 * geometry->size()))
		return false;

	// Otherwise create the eight children bounding boxes.
	for (int i = 0; i < 8; i++) {
		children[i] = new OctreeNode(childData[i]);
	}

	return true;
}

BoundingBox OctreeNode::createBoundingBox(const std::vector<std::shared_ptr<IGeometry>> *geometry) {
	BoundingBox result = BoundingBox();
	BoundingBox box;

	for (unsigned int i = 0; i < geometry->size(); i++)  {
		box = geometry->at(i)->getBoundingBox();

		result.includePoint(box.min);
		result.includePoint(box.max);
	}

	return result;
}

void OctreeNode::createChildBoundingBoxes(const BoundingBox &box, BoundingBox children[8]) {
	// 0 = (-1,-1,-1), ( 0, 0, 0)
	// 1 = ( 0,-1,-1), ( 1, 0, 0)
	// 3 = (-1, 0,-1), ( 0, 1, 0)
	// 2 = ( 0, 0,-1), ( 1, 1, 0)
	// 7 = (-1,-1, 0), ( 0, 0, 1)
	// 6 = ( 0,-1, 0), ( 1, 0, 1)
	// 4 = (-1, 0, 0), ( 0, 1, 1)
	// 5 = ( 0, 0, 0), ( 1, 1, 1)

	Vec3Df max = box.max;
	Vec3Df min = box.min;
	Vec3Df mid = box.getCenter();
	BoundingBox temp;

	temp.min = min;
	temp.max = mid;
	children[0] = temp;

	temp.min[0] = mid[0];
	temp.max[0] = max[0];
	children[1] = temp;

	temp.min[1] = mid[1];
	temp.max[1] = max[1];
	children[3] = temp;

	temp.min[0] = min[0];
	temp.max[0] = mid[0];
	children[2] = temp;

	temp.min[2] = mid[2];
	temp.max[2] = max[2];
	children[7] = temp;

	temp.min[0] = mid[0];
	temp.max[0] = max[0];
	children[6] = temp;

	temp.min[1] = min[1];
	temp.max[1] = mid[1];
	children[4] = temp;

	temp.min[0] = min[0];
	temp.max[0] = mid[0];
	children[5] = temp;
}