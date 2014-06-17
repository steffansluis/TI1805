#include <algorithm>
#include <limits>

#include "BoundingBox.h"

// TODO: Implement the bounding box functions here

BoundingBox::BoundingBox() {
}

BoundingBox::BoundingBox(const Vec3Df &min, const Vec3Df &max) {
}

bool BoundingBox::intersects(const Vec3Df &origin, const Vec3Df &dir) const {
	return true;
}

bool BoundingBox::intersects(const Vec3Df &origin, const Vec3Df &dir, float &distance) const {
	return true;
}

Vec3Df BoundingBox::getCenter() const {
	return Vec3Df();
}

void BoundingBox::includePoint(const Vec3Df &point) {
}