#include <algorithm>
#include <limits>

#include "BoundingBox.h"

// TODO: Implement the BoundingBox::intersects functions

BoundingBox::BoundingBox() :
min(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()),
max(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()) {
}

BoundingBox::BoundingBox(const Vec3Df &min, const Vec3Df &max)
: min(min), max(max) {
}

bool BoundingBox::intersects(const Vec3Df &origin, const Vec3Df &dir) const {
	float distance;

	return this->intersects(origin, dir, distance);
}

bool BoundingBox::intersects(const Vec3Df &origin, const Vec3Df &dir, float &distance) const {
	// TODO: Implement the intersection function
	return true;
}

Vec3Df BoundingBox::getCenter() const {
	return (this->min + this->max) * 0.5f;
}

void BoundingBox::includePoint(const Vec3Df &point) {
	// Find the new minimum bound
	this->min[0] = std::min<float>(this->min[0], point[0]);
	this->min[1] = std::min<float>(this->min[1], point[1]);
	this->min[2] = std::min<float>(this->min[2], point[2]);

	// Find the new maximum bound
	this->max[0] = std::max<float>(this->max[0], point[0]);
	this->max[1] = std::max<float>(this->max[1], point[1]);
	this->max[2] = std::max<float>(this->max[2], point[2]);
}