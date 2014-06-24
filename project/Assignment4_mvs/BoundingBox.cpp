#include <algorithm>
#include <limits>

#include "BoundingBox.h"

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
	float Tx_min = (this->min[0] - origin[0]) / dir[0];
	float Ty_min = (this->min[1] - origin[1]) / dir[1];
	float Tz_min = (this->min[2] - origin[2]) / dir[2];
	float Tx_max = (this->max[0] - origin[0]) / dir[0];
	float Ty_max = (this->max[1] - origin[1]) / dir[1];
	float Tz_max = (this->max[2] - origin[2]) / dir[2];
	
	float Tin_x = std::min<float>(Tx_min, Tx_max);
	float Tout_x = std::max<float>(Tx_min, Tx_max);
	float Tin_y = std::min<float>(Ty_min, Ty_max);
	float Tout_y = std::max<float>(Ty_min, Ty_max);
	float Tin_z = std::min<float>(Tz_min, Tz_max);
	float Tout_z = std::max<float>(Tz_min, Tz_max);
	
	float Tin = std::max<float>(Tin_x, std::max<float>(Tin_y, Tin_z));
	float Tout = std::min<float>(Tout_x, std::min<float>(Tout_y, Tout_z));
	
	if(Tin > Tout || Tout < 0){
		return false;
	}
	
	distance = Tin;
	
	return true;
}

bool BoundingBox::intersects(const BoundingBox &other) const {
	return 
		this->max[0] >= other.min[0] && this->min[0] <= other.max[0] &&
		this->max[1] >= other.min[1] && this->min[1] <= other.max[1] && 
		this->max[2] >= other.min[2] && this->min[2] <= other.max[2];
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