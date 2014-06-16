#include "IGeometry.h"
#include "IMaterial.h"
#include "SurfacePoint.h"

// TODO: Shading should really be placed here, calls should be delegated this->geometry->getMaterial()->... with the right parameters.

Vec3Df SurfacePoint::ambientLight() const {
	return this->geometry->getMaterial()->ambientLight(this);
}

Vec3Df SurfacePoint::emittedLight(const Vec3Df &outgoingVector) const {
	return this->geometry->getMaterial()->emittedLight(this, outgoingVector);
}

Vec3Df SurfacePoint::reflectedLight(const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const {
	return this->geometry->getMaterial()->reflectedLight(this, outgoingVector, incommingVector, lightColor);
}

Vec3Df SurfacePoint::specularLight(const Vec3Df &outgoingVector) const {
	return this->geometry->getMaterial()->specularLight(this, outgoingVector);
}