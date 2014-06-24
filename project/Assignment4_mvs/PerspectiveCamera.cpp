#include <cassert>

#include "Constants.h"
#include "PerspectiveCamera.h"
#include "Random.h"


PerspectiveCamera::PerspectiveCamera()
: ICamera(), fieldOfView(Constants::PiOver4) {
}
PerspectiveCamera::PerspectiveCamera(const Vec3Df &position, const Vec3Df &lookAt)
: ICamera(position, lookAt), fieldOfView(Constants::PiOver4) {
}
PerspectiveCamera::PerspectiveCamera(const Vec3Df &position, const Vec3Df &lookAt, const Vec3Df &up)
: ICamera(position, lookAt, up), fieldOfView(Constants::PiOver4) {
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
	float distance = 0.5f / tanf(0.5f * this->fieldOfView);

	//Values needed for depth of field

	//Distance to the image the focus if on
	focalDistance = this->getPosition().getLength() - this->getLookAt().getLength();
	//half width of the view plane
	halfWidth = distance*tan(fieldOfView / 2);
	//radius of the "eyeball", I will play with this a bit, i dont know what a good radius would be
	this->ApertureRadius = 10;
	this->xApertureRadius = this->right*ApertureRadius;
	this->yApertureRadius = this->up*ApertureRadius;
	


	// Offset from position to centre of the image plane
	this->imagePlaneOffset = this->forward * distance;
}

void PerspectiveCamera::getRay(float u, float v, Vec3Df &origin, Vec3Df &dir) const {
	// Get the vector towards centre of the image plane and offset it 
	// by the right and up vectors scaled by u and v respectivly.
	dir = this->imagePlaneOffset + this->right * u - this->up * v;

	// Calculate two random vectors for the offset to somewhere in our "eye"

	float r1, r2;


	Random::sampleUnitDisk(r1, r2);
	float R1 = r1;
	float R2 = r2;

	Vec3Df randomisedEyePoint = Vec3Df::addition(R1* this->xApertureRadius, R2*this->yApertureRadius);
	origin = randomisedEyePoint;
	origin.normalize();
	dir.normalize();

}