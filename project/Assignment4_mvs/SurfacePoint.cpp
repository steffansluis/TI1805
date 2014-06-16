#include "SurfacePoint.h"

// TODO: Shading should really be placed here, calls should be delegated this->geometry->getMaterial()->... with the right parameters.

Vec3Df SurfacePoint::ambientLight() const {
	return Vec3Df(0.1f, 0.1f, 0.1f);
}

Vec3Df SurfacePoint::emittedLight(const Vec3Df &outgoingVector) const {
	return Vec3Df();
}

Vec3Df SurfacePoint::reflectedLight(const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const {
	// Temporary diffuse shader
	float dot_normal_light = -Vec3Df::dotProduct(this->normal, incommingVector);

	if (dot_normal_light < 0.0f)
		dot_normal_light = 0.0f;

	return dot_normal_light * lightColor;
}

Vec3Df SurfacePoint::specularLight(const Vec3Df &outgoingVector) const {
	return Vec3Df();
}