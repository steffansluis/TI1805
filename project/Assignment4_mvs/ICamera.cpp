#include <cassert>

#include "ICamera.h"

ICamera::ICamera() 
: position(0, 0, 0), lookAt(0, 0, 1), up(0, 1, 0) {
}
ICamera::ICamera(const Vec3Df &position, const Vec3Df &lookAt) 
: position(position), lookAt(lookAt), up(0, 1, 0) {
}
ICamera::~ICamera() {
}

float ICamera::getAspectRatio() const {
	return this->aspectRatio;
}
Vec3Df ICamera::getLookAt() const {
	return this->lookAt;
}
Vec3Df ICamera::getPosition() const {
	return this->position;
}
Vec3Df ICamera::getUp() const {
	return this->up;
}

void ICamera::setLookAt(const Vec3Df &lookAt) {
	this->lookAt = lookAt;
}
void ICamera::setPosition(const Vec3Df &position) {
	this->position = position;
}
void ICamera::setUp(const Vec3Df &up) {
	this->up = up;
	this->up.normalize();
}

void ICamera::getRay(int x, int y, Vec3Df &origin, Vec3Df &dir) const {
	return this->getRay(x, y, 0, 0, origin, dir);
}
void ICamera::getRay(int x, int y, float subPixelX, float subPixelY, Vec3Df &origin, Vec3Df &dir) const {
	assert(x >= 0 && x <= (1.0 / this->invWidth));
	assert(y >= 0 && y <= (1.0 / this->invHeight));

	// Maps the pixel to the virtual image plane ([-0.5, 0.5] x [-0.5, 0.5]).
	return this->getRay(
		-0.5f + x * this->invWidth + subPixelX * this->invWidth,
		-0.5f + y * this->invHeight + subPixelY * this->invHeight,
		origin,
		dir);
}

void ICamera::preprocess(int width, int height) {
	this->invWidth = 1.0f / width;
	this->invHeight = 1.0f / height;
	this->aspectRatio = width * this->invHeight;
}