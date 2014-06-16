#include <assert.h>

#include "ConstantTexture.h"
#include "DiffuseMaterial.h"
#include "ITexture.h"
#include "SurfacePoint.h"

DiffuseMaterial::DiffuseMaterial() 
: DiffuseMaterial(Vec3Df(1, 1, 1)) {
}

DiffuseMaterial::DiffuseMaterial(const Vec3Df &color) {
	static auto texture = std::make_shared<ConstantTexture>(color);

	this->setAmbientTexture(texture);
	this->setDiffuseTexture(texture);
}

DiffuseMaterial::~DiffuseMaterial() {
}

Vec3Df DiffuseMaterial::reflectedLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const {
	assert(surface);

	// !!! TEMPORARY
	// Lambertian shader, abstract this behind a BRDF.
	float dot_normal_light = -Vec3Df::dotProduct(surface->normal, incommingVector);

	if (dot_normal_light < 0.0f)
		dot_normal_light = 0.0f;

	Vec3Df diffuseColor = this->getDiffuseTexture()->sample(surface->u, surface->v);

	return dot_normal_light * lightColor * diffuseColor;
}

Vec3Df DiffuseMaterial::specularLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector) const {
	assert(surface);

	// This is a diffuse material, no specular light is reflected
	return Vec3Df(0, 0, 0);
}