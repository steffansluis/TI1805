#include <cassert>

#include "Constants.h"
#include "PerspectiveCamera.h"
#include "Random.h"

PerspectiveCamera::PerspectiveCamera()
: ICamera(), fieldOfView(Constants::PiOver4), ApertureRadius(0.0f), focalDistance(0.0f) {
}
PerspectiveCamera::PerspectiveCamera(const Vec3Df &position, const Vec3Df &lookAt)
: ICamera(position, lookAt), fieldOfView(Constants::PiOver4), ApertureRadius(0.0f), focalDistance(0.0f) {
}
PerspectiveCamera::PerspectiveCamera(const Vec3Df &position, const Vec3Df &lookAt, const Vec3Df &up)
: ICamera(position, lookAt, up), fieldOfView(Constants::PiOver4), ApertureRadius(0.0f), focalDistance(0.0f) {
}

float PerspectiveCamera::getAperatureRadius() const {
	return this->ApertureRadius;
}
float PerspectiveCamera::getFieldOfView() const {
	return this->fieldOfView;
}
float PerspectiveCamera::getFocalDistance() const {
	return this->focalDistance;
}
void PerspectiveCamera::setAperatureRadius(float radius) {
	assert(radius >= 0.0f);

	this->ApertureRadius = radius;
}
void PerspectiveCamera::setFieldOfView(float fov) {
	assert(fov >= 0.0f && fov <= Constants::TwoPi);

	this->fieldOfView = fov;
}
void PerspectiveCamera::setFocalDistance(float focalDistance) {
	assert(focalDistance >= 0.0f);

	this->focalDistance = focalDistance;
}

void PerspectiveCamera::preprocess(int width, int height) {
	ICamera::preprocess(width, height);

	// Create orthonormal basis
	this->forward = this->getLookAt() - this->getPosition();
	this->forward.normalize();
	this->right = Vec3Df::crossProduct(this->forward, ICamera::getUp());
	this->right.normalize();
	this->up = Vec3Df::crossProduct(this->right, this->forward);

	// Find the distance from the eye to the virtual image plane
	float distance = 0.5f / tanf(0.5f * this->fieldOfView);

	// Distance to the focal plane
	// If no default value is set, use the distance towards the camera target
	if (this->focalDistance <= 0.0f)  {
		this->focalDistance = (this->getPosition() - this->getLookAt()).getLength();
	}

	// Radius of the circle of confusion
	this->xApertureRadius = this->right * this->ApertureRadius;
	this->yApertureRadius = this->up * this->ApertureRadius;
	
	// Scale the right vector by the aspect ratio
	this->right *= this->getAspectRatio();

	// Offset from position to centre of the image plane
	this->imagePlaneOffset = this->forward * distance;
}

void PerspectiveCamera::getRay(float u, float v, Vec3Df &origin, Vec3Df &dir) const
{
	// Calculate two random vectors for the offset to somewhere in our "eye"
	float r1;
	float r2;
	Random::sampleUnitDisk(r1, r2);

	// Offset the ray origin to a point in the circle of confusion
	origin = this->getPosition() + r1 * this->xApertureRadius + r2 * this->yApertureRadius;

	// Trace a ray from the center of the lens through the image plane into the scene
	dir = this->getPosition() + (this->imagePlaneOffset + this->right * u - this->up * v) * this->focalDistance;

	// Get the direction from the point in the circle of confusion to the point in the scene
	dir -= origin;
	dir.normalize();
}