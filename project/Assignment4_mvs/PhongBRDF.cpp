#include <algorithm>
#include <cassert>
#include <cmath>

#include "IMaterial.h"
#include "ITexture.h"
#include "PhongBRDF.h"

PhongBRDF::PhongBRDF(const IMaterial * material)
: BRDF(material) {
	// Nothing to do here
}

/**
	The Phong BRDF.
	Implemented suing the formulaes described in http://en.wikipedia.org/wiki/Phong_reflection_model

	For now every object's material defaults to DiffuseMaterial, the DiffuseMaterial uses the LambertianBRDF by default.
    If you want to test a different brdf, modifiy the DiffuseMaterial(color, roughness) constructor to set diffuseBrdf to whichever one you need to test.
    DiffuseMaterial.cpp also contains the default color and roughness, change these as you wish.

	@author Joren Hammudoglu
*/
Vec3Df PhongBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	Vec3Df Lm = incommingVector;
	Lm.normalize();

	Vec3Df Nm = normal;
	Nm.normalize();

	Vec3Df Rm = 2 * Vec3Df::dotProduct(Lm, Nm) * Nm - Lm;
	Rm.normalize();

	Vec3Df V = reflectedVector;
	V.normalize();

	float VdotR = std::max<float>(0, Vec3Df::dotProduct(Rm, V));
	return this->material->sampleColor(texCoords) * std::pow(VdotR, this->material->getShininess());
}
