#include <cassert>
#include <type_traits>

#include "BRDF.h"
#include "Constants.h"
#include "ConstantTexture.h"
#include "IMaterial.h"
#include "IRayTracer.h"
#include "ITexture.h"
#include "Reflection.h"
#include "Scene.h"
#include "SurfacePoint.h"

#include "LambertianBRDF.h"
#include "OrenNayarBRDF.h"
#include "BlinnPhongBRDF.h"
#include "PhongBRDF.h"

IMaterial::IMaterial() :
texture(nullptr),
ambientCoefficient(1.0f),
diffuseCoefficient(1.0f),
emissiveness(0.0f),
reflectiveness(0.0f),
transparency(0.0f),
absorbance(0.0f),
roughness(0.0f),
shininess(45.0f),
refractiveIndex(Constants::AirRefractiveIndex),
brdf(NULL),
reflection(NULL)
{
	static auto white = std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1));

	//std::shared_ptr<const BRDF> lambertianBrdf = std::make_shared<LambertianBRDF>(this);
	//std::shared_ptr<const BRDF> orenNayarBrdf = std::make_shared<LambertianBRDF>(this);
	//std::shared_ptr<const BRDF> blinnPhongBrdf = std::make_shared<BlinnPhongBRDF>(this);
	//std::shared_ptr<const BRDF> phongBrdf = std::make_shared<PhongBRDF>(this);

	this->setTexture(white);
	this->setBRDF<LambertianBRDF>();
}
IMaterial::~IMaterial() {
}

std::shared_ptr<const ITexture> IMaterial::getTexture() const{
	return this->texture;
}
float IMaterial::getAmbientCoefficient() const {
	return this->ambientCoefficient;
}
float IMaterial::getDiffuseCoefficient() const {
	return this->diffuseCoefficient;
}
float IMaterial::getEmissiveness() const {
	return this->emissiveness;
}
float IMaterial::getReflectiveness() const {
	return this->reflectiveness;
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
void IMaterial::setAmbientCoefficient(float ambientCoefficient) {
	assert(ambientCoefficient >= 0.0f && ambientCoefficient <= 1.0f);

	this->ambientCoefficient = ambientCoefficient;
}
void IMaterial::setDiffuseCoefficient(float diffuseCoefficient) {
	assert(diffuseCoefficient >= 0.0f && diffuseCoefficient <= 1.0f);

	this->diffuseCoefficient = diffuseCoefficient;
}
void IMaterial::setEmissiveness(float emissiveness) {
	assert(emissiveness >= 0.0f && emissiveness <= 1.0f);

	this->emissiveness = emissiveness;
}
void IMaterial::setReflectiveness(float reflectiveness) {
	assert(reflectiveness >= 0.0f && reflectiveness <= 1.0f);

	this->reflectiveness = reflectiveness;
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
void  IMaterial::setBRDF() {
	static_assert(std::is_base_of<BRDF, T>::value, "Type T must be subclass of BRDF");

	if (this->brdf) {
		delete this->brdf;
	}

	this->brdf = new T(this);
}

template<class T>
void  IMaterial::setReflection() {
	static_assert(std::is_base_of<Reflection, T>);

	if (this->reflection) {
		delete this->reflection;
	}

	this->reflection = new T(this);
}

Vec3Df IMaterial::ambientLight(const SurfacePoint &surface, const Scene *scene) const {
	return scene->getAmbientLight() * this->ambientCoefficient;
}

Vec3Df IMaterial::emittedLight(const SurfacePoint &surface, const Vec3Df &reflectedVector) const {
	return this->sampleColor(surface.texCoords) * this->emissiveness;
}

Vec3Df IMaterial::reflectedLight(const SurfacePoint &surface, const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &lightColor) const {
	if (this->brdf) {
		// If we have a BRDF set, sample its reflectance
		return this->brdf->reflectance(incommingVector, reflectedVector, surface.normal, surface.texCoords, lightColor);
	}
	else {
		return Vec3Df();
	}
}

Vec3Df IMaterial::specularLight(
	const SurfacePoint &surface, 
	const Vec3Df &reflectedVector,
	const Scene *scene,
	int iteration) const
{
	// If the material is reflective or transparent...
	if (this->reflection && (this->reflectiveness || this->transparency)) {
		Vec3Df incommingVector = reflectedVector;
		Vec3Df reflectedVector;
		Vec3Df refractedVector;
		Vec3Df reflectance;
		Vec3Df transmittance;
		Vec3Df result = Vec3Df();
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

		// Sample the relfected vector, reflectance, refracted vector and tranmittance
		this->reflection->sample(
			incommingVector, 
			surface.normal, 
			surface.texCoords,
			n1,
			n2,
			reflectedVector,
			reflectance, 
			refractedVector,
			transmittance);

		if (this->reflectiveness && (reflectance[0] || reflectance[1] || reflectance[2])) {
			float distance;

			// Trace the reflection ray
			result += this->reflectiveness * reflectance * scene->getRayTracer()->performRayTracingIteration(
				surface.point + reflectedVector * Constants::Epsilon,
				reflectedVector,
				iteration + 1,
				distance);
		}

		if (this->transparency && (transmittance[0] || transmittance[1] || transmittance[2])) {
			float distance;

			// Trace the refraction ray
			Vec3Df tranmitted = transmittance * scene->getRayTracer()->performRayTracingIteration(
				surface.point + refractedVector * Constants::Epsilon,
				refractedVector,
				iteration + 1,
				distance);

			// Absorbance using beer's law
			Vec3Df absorbance = this->sampleColor(surface.texCoords) * -distance;
			absorbance[0] = expf(absorbance[0]);
			absorbance[1] = expf(absorbance[2]);
			absorbance[2] = expf(absorbance[1]);

			result += this->transparency * tranmitted * absorbance;
		}

		return result;
	}
	else {
		return Vec3Df();
	}
}