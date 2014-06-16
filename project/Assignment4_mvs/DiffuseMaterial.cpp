#include "ConstantTexture.h"
#include "DiffuseMaterial.h"
#include "ITexture.h"
#include "SurfacePoint.h"

DiffuseMaterial::DiffuseMaterial() {
	// !!! TEMPORARY
	this->setDiffuseTexture(new ConstantTexture(Vec3Df(1, 0, 0)));
}

DiffuseMaterial::~DiffuseMaterial() {
	// !!! TEMPORARY
	delete this->getDiffuseTexture();
}

Vec3Df DiffuseMaterial::reflectedLight(const SurfacePoint *surface, const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const {
	// !!! TEMPORARY
	// Lambertian shader, abstract this behind a BRDF.
	float dot_normal_light = -Vec3Df::dotProduct(surface->normal, incommingVector);

	if (dot_normal_light < 0.0f)
		dot_normal_light = 0.0f;

	Vec3Df diffuseColor = this->getDiffuseTexture()->sample(surface->u, surface->v);

	return dot_normal_light * lightColor * diffuseColor;
}

Vec3Df DiffuseMaterial::specularLight(const SurfacePoint *surface, const Vec3Df &outgoingVector) const {
	// This is a diffuse material, no specular light is reflected
	return Vec3Df(0, 0, 0);
}