#include <assert.h>

#include "Constants.h"
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
: ICamera(), fieldOfView(Constants::PiOver4) {
}
PerspectiveCamera::PerspectiveCamera(const Vec3Df &position, const Vec3Df &lookAt)
: ICamera(position, lookAt), fieldOfView(Constants::PiOver4) {
}

float PerspectiveCamera::getFieldOfView() const {
	return this->fieldOfView;
}
void PerspectiveCamera::setFieldOfView(float fov) {
	assert(fov >= 0.0f && fov <= Constants::TwoPi);

	this->fieldOfView = fov;
}

void PerspectiveCamera::preprocess(int width, int height) {
	ICamera::preprocess(width, height);

	// Create orthonormal basis
	this->forward = this->getLookAt() - this->getPosition();
	this->forward.normalize();
	this->right = Vec3Df::crossProduct(this->forward, ICamera::getUp());
	this->right.normalize();
	this->up = Vec3Df::crossProduct(this->right, this->forward);

	// Scale the right vector by the aspect ratio
	this->right *= this->getAspectRatio();

	// Find the distance from the eye to the virtual image plane
	// TODO: Think and verify this, tired atm
	float distance = 0.5 / tanf(0.5f * this->fieldOfView);

	// Offset from position to centre of the image plane
	this->imagePlaneOffset = this->forward * distance;
}

void PerspectiveCamera::getRay(float u, float v, Vec3Df &origin, Vec3Df &dir) const {
	// Get the vector towards centre of the image plane and offset it 
	// by the right and up vectors scaled by u and v respectivly.
	dir = this->imagePlaneOffset + this->right * u - this->up * v;
	dir.normalize();

	origin = this->getPosition();
}