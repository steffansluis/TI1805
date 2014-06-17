#include <cassert>

#include "BRDF.h"
#include "LambertianBRDF.h"
#include "OrenNayarBRDF.h"
#include "BlinnPhongBRDF.h"
#include "PhongBRDF.h"

#include "ConstantTexture.h"
#include "DiffuseMaterial.h"
#include "ITexture.h"
#include "SurfacePoint.h"

// Default settings, you can change these for testing
static Vec3Df defaultColor = Vec3Df(1, 1, 1);
static float defaultRoughness = 0.0f;

DiffuseMaterial::DiffuseMaterial() 
: DiffuseMaterial(::defaultColor, defaultRoughness) {
}

DiffuseMaterial::DiffuseMaterial(const Vec3Df &color)
: DiffuseMaterial(color, defaultRoughness) {
}

DiffuseMaterial::DiffuseMaterial(const Vec3Df &color, float roughness) {
	auto colorTexture = std::make_shared<ConstantTexture>(color);
	auto roughnessTexture = std::make_shared<ConstantTexture>(Vec3Df(roughness, 0, 0));
	
	std::shared_ptr<const BRDF> lambertianBrdf = std::make_shared<LambertianBRDF>(this);
	std::shared_ptr<const BRDF> orenNayarBrdf = std::make_shared<LambertianBRDF>(this);
	std::shared_ptr<const BRDF> blinnPhongBrdf = std::make_shared<BlinnPhongBRDF>(this);
	std::shared_ptr<const BRDF> phongBrdf = std::make_shared<PhongBRDF>(this);

	// Change BRDF to whichever you need to test here, for now
	this->diffuseBrdf = lambertianBrdf;

	this->setAmbientColor(colorTexture);
	this->setDiffuseColor(colorTexture);
	this->setRoughness(roughnessTexture);
}

DiffuseMaterial::~DiffuseMaterial() {
}

Vec3Df DiffuseMaterial::reflectedLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &lightColor) const {
	assert(surface);

	return this->diffuseBrdf->evaluate(incommingVector, outgoingVector, surface->normal, surface->texCoords) * lightColor;
}

Vec3Df DiffuseMaterial::specularLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector, const Scene *) const {
	assert(surface);

	// This is a diffuse material, no specular light is reflected
	return Vec3Df(0, 0, 0);
}