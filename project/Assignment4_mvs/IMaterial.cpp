#include <cassert>
#include <type_traits>

#include "BRDF.h"
#include "Constants.h"
#include "ConstantTexture.h"
#include "IMaterial.h"
#include "IRayTracer.h"
#include "ITexture.h"
#include "Scene.h"
#include "SurfacePoint.h"

#include "LambertianBRDF.h"
#include "OrenNayarBRDF.h"
#include "BlinnPhongBRDF.h"
#include "PhongBRDF.h"

IMaterial::IMaterial() :
texture(nullptr),
ambientReflectance(1.0f),
diffuseReflectance(1.0f),
specularReflectance(1.0f),
emissiveness(0.0f),
transparency(0.0f),
absorbance(0.0f),
roughness(0.0f),
shininess(45.0f),
refractiveIndex(Constants::AirRefractiveIndex),
diffuseBrdf(NULL),
specularBrdf(NULL)
{
	static auto white = std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1));

	this->setTexture(white);
	this->setDiffuseBRDF<OrenNayarBRDF>();
	this->setSpecularBRDF<BlinnPhongBRDF>();
}
IMaterial::~IMaterial() {
}

std::shared_ptr<const ITexture> IMaterial::getTexture() const{
	return this->texture;
}
float IMaterial::getAmbientReflectance() const {
	return this->ambientReflectance;
}
float IMaterial::getDiffuseReflectance() const {
	return this->diffuseReflectance;
}
float IMaterial::getSpecularReflectance() const {
	return this->specularReflectance;
}
float IMaterial::getEmissiveness() const {
	return this->emissiveness;
}
float IMaterial::getTransparency() const {
	return this->transparency;
}
float IMaterial::getAbsorbance() const {
	return this->absorbance;
}
float IMaterial::getRoughness() const {
	return this->roughness;
}
float IMaterial::getShininess() const {
	return this->shininess;
}

void IMaterial::setTexture(std::shared_ptr<const ITexture> texture) {
	assert(texture);
	
	this->texture = texture;
}
void IMaterial::setAmbientReflectance(float ambientReflectance) {
	assert(ambientReflectance >= 0.0f && ambientReflectance <= 1.0f);

	this->ambientReflectance = ambientReflectance;
}
void IMaterial::setDiffuseReflectance(float diffuseReflectance) {
	assert(diffuseReflectance >= 0.0f && diffuseReflectance <= 1.0f);

	this->diffuseReflectance = diffuseReflectance;
}
void IMaterial::setSpecularReflectance(float specularReflectance) {
	assert(specularReflectance >= 0.0f && specularReflectance <= 1.0f);

	this->specularReflectance = specularReflectance;
}
void IMaterial::setEmissiveness(float emissiveness) {
	assert(emissiveness >= 0.0f && emissiveness <= 1.0f);

	this->emissiveness = emissiveness;
}
void IMaterial::setTransparency(float transparency) {
	assert(transparency >= 0.0f && transparency <= 1.0f);

	this->transparency = transparency;
}
void IMaterial::setAbsorbance(float absorbance) {
	assert(absorbance >= 0.0f);

	this->absorbance = absorbance;
}
void IMaterial::setRoughness(float roughness) {
	assert(roughness >= 0.0f && roughness <= 1.0f);

	this->roughness = roughness;
}
void IMaterial::setShininess(float shininess) {
	assert(shininess >= 0.0f);

	this->shininess = shininess;
}

Vec3Df IMaterial::sampleColor(const Vec2Df &texCoords) const {
	return this->texture->sample(texCoords);
}

template<class T>
void  IMaterial::setDiffuseBRDF() {
	static_assert(std::is_base_of<BRDF, T>::value, "Type T must be subclass of BRDF");

	if (this->diffuseBrdf) {
		delete this->diffuseBrdf;
	}

	this->diffuseBrdf = new T(this);
}

template<class T>
void  IMaterial::setSpecularBRDF() {
	static_assert(std::is_base_of<BRDF, T>::value, "Type T must be subclass of BRDF");

	if (this->specularBrdf) {
		delete this->specularBrdf;
	}

	this->specularBrdf = new T(this);
}

Vec3Df IMaterial::ambientLight(const SurfacePoint &surface, const Scene *scene) const {
	return this->sampleColor(surface.texCoords) * this->ambientReflectance * scene->getAmbientLight();
}

Vec3Df IMaterial::emittedLight(const SurfacePoint &surface, const Vec3Df &reflectedVector) const {
	return this->sampleColor(surface.texCoords) * this->emissiveness;
}

Vec3Df IMaterial::reflectedLight(const SurfacePoint &surface, const Vec3Df &incomingVector, const Vec3Df &reflectedVector, const Vec3Df &lightColor) const {
	Vec3Df result = Vec3Df();

	if (this->diffuseBrdf) {
		// If we have a diffuse BRDF set, sample its reflectance
		result += this->diffuseReflectance * this->diffuseBrdf->reflectance(incomingVector, reflectedVector, surface.normal, surface.texCoords, lightColor);
	}

	if (this->specularBrdf) {
		// If we have a specular BRDF set, sample its reflectance
		result += this->specularReflectance * this->specularBrdf->reflectance(incomingVector, reflectedVector, surface.normal, surface.texCoords, lightColor);
	}

	return result;
}

Vec3Df IMaterial::specularLight(
	const SurfacePoint &surface, 
	const Vec3Df &reflectedVector,
	const Scene *scene,
	int iteration) const
{
	// If the material is transparent...
	if (this->specularReflectance > 0.0f) {
		Vec3Df incomingVector = reflectedVector;

		// Calculate the reflected vector
		Vec3Df reflectedVector = IMaterial::calculateReflectionVector(incomingVector, surface.normal);

		// Trace the reflection ray
		float distance;
		Vec3Df reflected = scene->getRayTracer()->performRayTracingIteration(
			surface.point + reflectedVector * Constants::Epsilon,
			reflectedVector,
			iteration + 1,
			distance);

		// Calculate the reflectance
		Vec3Df reflectance = this->sampleColor(surface.texCoords);

		return this->specularReflectance * reflected * reflectance;
	}
	else {
		return Vec3Df();
	}
}

Vec3Df IMaterial::transmittedLight(
	const SurfacePoint &surface,
	const Vec3Df &reflectedVector,
	const Scene *scene,
	int iteration) const
{
	// If the material is transparent...
	if (this->transparency > 0.0f) {
		Vec3Df incomingVector = reflectedVector;
		float n1, n2;

		// Get the refractive indices	
		if (surface.isInside) {
			n1 = this->refractiveIndex;
			n2 = Constants::AirRefractiveIndex;
		}
		else {
			n1 = Constants::AirRefractiveIndex;
			n2 = this->refractiveIndex;
		}

		// Sample the relfected vector, reflectance, refracted vector and transmittance
		Vec3Df refractedVector = IMaterial::calculateRefractedVector(incomingVector, surface.normal, n1, n2);

		float distance;

		// Trace the refraction ray
		Vec3Df transmitted = scene->getRayTracer()->performRayTracingIteration(
			surface.point + refractedVector * Constants::Epsilon,
			refractedVector,
			iteration + 1,
			distance);

		// Absorbance using beer's law
		Vec3Df absorbance = this->sampleColor(surface.texCoords) * -distance;
		absorbance[0] = expf(absorbance[0]);
		absorbance[1] = expf(absorbance[2]);
		absorbance[2] = expf(absorbance[1]);

		return this->transparency * transmitted * absorbance;
	}
	else {
		return Vec3Df();
	}
}

Vec3Df IMaterial::calculateReflectionVector(
	const Vec3Df &incomingVector,
	const Vec3Df &normal) {
	
	Vec3Df IdotN = Vec3Df::dotProduct(incomingVector, normal);
	
	/* It doesn't work if incomingVector and normal are orthogonal to each other
	if(IdotN < 0)) {
		return NULL;
	}*/
	return incomingVector - (2 * IdotN * normal);
}

Vec3Df IMaterial::calculateRefractedVector(
	const Vec3Df &incomingVector,
	const Vec3Df &normal,
	float n1,
	float n2) {
	return Vec3Df();
}