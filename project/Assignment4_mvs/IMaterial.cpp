#include <cassert>

#include "ConstantTexture.h"
#include "IMaterial.h"
#include "ITexture.h"
#include "SurfacePoint.h"

IMaterial::IMaterial() {
	static auto black = std::make_shared<ConstantTexture>(Vec3Df(0, 0, 0));

	this->setAmbientColor(black);
	this->setDiffuseColor(black);
	this->setEmissiveColor(black);
	this->setSpecularColor(black);
	this->setRoughness(black);
	this->setShininess(black);
}
IMaterial::~IMaterial() {
}

std::shared_ptr<const ITexture> IMaterial::getAmbientColor() const {
	return this->ambientTexture;
}
std::shared_ptr<const ITexture> IMaterial::getDiffuseColor() const {
	return this->diffuseTexture;
}
std::shared_ptr<const ITexture> IMaterial::getEmissiveColor() const {
	return this->emissiveTexture;
}
std::shared_ptr<const ITexture> IMaterial::getSpecularColor() const {
	return this->specularTexture;
}
std::shared_ptr<const ITexture> IMaterial::getRoughness() const {
	return this->roughness;
}
std::shared_ptr<const ITexture> IMaterial::getShininess() const {
	return this->shininess;
}

void IMaterial::setAmbientColor(std::shared_ptr<const ITexture> texture) {
	assert(texture);

	this->ambientTexture = texture;
}
void IMaterial::setDiffuseColor(std::shared_ptr<const ITexture> texture) {
	assert(texture);

	this->diffuseTexture = texture;
}
void IMaterial::setEmissiveColor(std::shared_ptr<const ITexture> texture) {
	assert(texture);

	this->emissiveTexture = texture;
}
void IMaterial::setSpecularColor(std::shared_ptr<const ITexture> texture) {
	assert(texture);

	this->specularTexture = texture;
}
void IMaterial::setRoughness(std::shared_ptr<const ITexture> texture) {
	assert(texture);

	this->roughness = texture;
}
void IMaterial::setShininess(std::shared_ptr<const ITexture> texture) {
	assert(texture);

	this->roughness = texture;
}

Vec3Df IMaterial::sampleAmbientColor(const Vec2Df &texCoords) const {
	return this->ambientTexture->sample(texCoords);
}
Vec3Df IMaterial::sampleDiffuseColor(const Vec2Df &texCoords) const {
	return this->diffuseTexture->sample(texCoords);
}
Vec3Df IMaterial::sampleEmissiveColor(const Vec2Df &texCoords) const {
	return this->emissiveTexture->sample(texCoords);
}
Vec3Df IMaterial::sampleSpecularColor(const Vec2Df &texCoords) const {
	return this->specularTexture->sample(texCoords);
}
float IMaterial::sampleRoughness(const Vec2Df &texCoords) const {
	return this->roughness->sample(texCoords)[0];
}
float IMaterial::sampleShininess(const Vec2Df &texCoords) const {
	return this->shininess->sample(texCoords)[0];
}

Vec3Df IMaterial::ambientLight(const SurfacePoint &surface, const Scene *scene) const {
	// TODO: Implement ambient lighting / occlusion.
	return Vec3Df();
}

Vec3Df IMaterial::emittedLight(const SurfacePoint &surface, const Vec3Df &reflectedVector) const {
	return this->emissiveTexture->sample(surface.texCoords);

	// TODO: Implement emitted light
	return Vec3Df();
}