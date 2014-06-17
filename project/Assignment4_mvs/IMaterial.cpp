#include <cassert>

#include "ConstantTexture.h"
#include "IMaterial.h"
#include "ITexture.h"
#include "SurfacePoint.h"

static auto black = std::make_shared<ConstantTexture>(Vec3Df(0, 0, 0));

IMaterial::IMaterial() {
	this->setAmbientTexture(black);
	this->setDiffuseTexture(black);
	this->setEmissiveTexture(black);
	this->setSpecularTexture(black);
}
IMaterial::~IMaterial() {
}

std::shared_ptr<const ITexture> IMaterial::getAmbientTexture() const {
	return this->ambientTexture;
}
std::shared_ptr<const ITexture> IMaterial::getDiffuseTexture() const {
	return this->diffuseTexture;
}
std::shared_ptr<const ITexture> IMaterial::getEmissiveTexture() const {
	return this->emissiveTexture;
}
std::shared_ptr<const ITexture> IMaterial::getSpecularTexture() const {
	return this->specularTexture;
}

void IMaterial::setAmbientTexture(std::shared_ptr<const ITexture> texture) {
	if (texture)
		this->ambientTexture = texture;
	else
		this->ambientTexture = black;
}
void IMaterial::setDiffuseTexture(std::shared_ptr<const ITexture> texture) {
	if (texture)
		this->diffuseTexture = texture;
	else
		this->diffuseTexture = black;
}
void IMaterial::setEmissiveTexture(std::shared_ptr<const ITexture> texture) {
	if (texture)
		this->emissiveTexture = texture;
	else
		this->emissiveTexture = black;
}
void IMaterial::setSpecularTexture(std::shared_ptr<const ITexture> texture) {
	if (texture)
		this->specularTexture = texture;
	else
		this->specularTexture = black;
}

Vec3Df IMaterial::ambientLight(std::shared_ptr<const SurfacePoint> surface, std::shared_ptr<const Scene> scene) const {
	assert(surface);

	// TODO: Implement ambient lighting / occlusion.
	return Vec3Df();
}

Vec3Df IMaterial::emittedLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector) const {
	assert(surface);

	// TODO: Implement emitted light
	return Vec3Df();
}