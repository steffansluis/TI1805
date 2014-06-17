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

// Default white texture
static std::shared_ptr<const ITexture> defaultTexture = std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1));
static float roughness = 0.3f;
static float shininess = 45.0f;

static std::shared_ptr<const BRDF> lambertianBrdf = std::make_shared<LambertianBRDF>(defaultTexture);
static std::shared_ptr<const BRDF> orenNayarBrdf = std::make_shared<LambertianBRDF>(defaultTexture, roughness);
static std::shared_ptr<const BRDF> blinnPhongBrdf = std::make_shared<BlinnPhongBRDF>(defaultTexture, shininess);
static std::shared_ptr<const BRDF> phongBrdf = std::make_shared<PhongBRDF>(defaultTexture, shininess);

// Change the lambertianBrdf to whatever BRDF you want to test
static std::shared_ptr<const BRDF> defaultBrdf = lambertianBrdf;

DiffuseMaterial::DiffuseMaterial() 
: DiffuseMaterial(Vec3Df(1, 1, 1)) {
}

DiffuseMaterial::DiffuseMaterial(const Vec3Df &color) 
: diffuseBrdf(defaultBrdf) {
	auto texture = std::make_shared<ConstantTexture>(color);

	this->setAmbientTexture(texture);
	this->setDiffuseTexture(texture);
}

DiffuseMaterial::~DiffuseMaterial() {
}

Vec3Df DiffuseMaterial::reflectedLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &lightColor) const {
	assert(surface);

	return this->diffuseBrdf->evaluate(incommingVector, outgoingVector, surface->normal, surface->texCoords) * lightColor;
}

Vec3Df DiffuseMaterial::specularLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector, std::shared_ptr<const Scene> scene) const {
	assert(surface);

	// This is a diffuse material, no specular light is reflected
	return Vec3Df(0, 0, 0);
}