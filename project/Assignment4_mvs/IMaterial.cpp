#include <assert.h>

#include "IMaterial.h"
#include "ITexture.h"

IMaterial::~IMaterial() {
}

const ITexture *IMaterial::getAmbientTexture() const {
	return this->ambientTexture;
}
const ITexture *IMaterial::getDiffuseTexture() const {
	return this->diffuseTexture;
}
const ITexture *IMaterial::getEmissiveTexture() const {
	return this->emissiveTexture;
}
const ITexture *IMaterial::getSpecularTexture() const {
	return this->specularTexture;
}

const ITexture *IMaterial::setAmbientTexture(const ITexture *texture) {
	assert(texture);
	
	const ITexture *oldTexture = this->ambientTexture;

	this->ambientTexture = texture;

	return oldTexture;
}
const ITexture *IMaterial::setDiffuseTexture(const ITexture *texture) {
	assert(texture);

	const ITexture *oldTexture = this->diffuseTexture;

	this->diffuseTexture = texture;

	return oldTexture;
}
const ITexture *IMaterial::setEmissiveTexture(const ITexture *texture) {
	assert(texture);

	const ITexture *oldTexture = this->emissiveTexture;

	this->emissiveTexture = texture;

	return oldTexture;
}
const ITexture *IMaterial::setSpecularTexture(const ITexture *texture) {
	assert(texture);

	const ITexture *oldTexture = this->specularTexture;

	this->specularTexture = texture;

	return oldTexture;
}

Vec3Df IMaterial::ambientLight(const SurfacePoint *surface) const {
	// TODO: Implement ambient lighting / occlusion.

	return Vec3Df();
}

Vec3Df IMaterial::emittedLight(const SurfacePoint *surface, const Vec3Df &outgoingVector) const {
	// TODO: Implement emitted light

	return Vec3Df();
}