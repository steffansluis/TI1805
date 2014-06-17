#include "IGeometry.h"
#include "IMaterial.h"
#include "Scene.h"
#include "SurfacePoint.h"

Vec3Df SurfacePoint::ambientLight(const Scene *scene) const {
	return this->geometry->getMaterial()->ambientLight(this->shared_from_this(), scene);
}

Vec3Df SurfacePoint::emittedLight(const Vec3Df &outgoingVector) const {
	return this->geometry->getMaterial()->emittedLight(this->shared_from_this(), outgoingVector);
}

Vec3Df SurfacePoint::reflectedLight(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &lightColor) const {
	return this->geometry->getMaterial()->reflectedLight(this->shared_from_this(), incommingVector, outgoingVector, lightColor);
}

Vec3Df SurfacePoint::specularLight(const Vec3Df &outgoingVector, const Scene *scene) const {
	return this->geometry->getMaterial()->specularLight(this->shared_from_this(), outgoingVector, scene);
}