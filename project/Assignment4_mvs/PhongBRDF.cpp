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
	Implemented using the formulaes described in http://en.wikipedia.org/wiki/Phong_reflection_model

	@author Joren Hammudoglu
*/
Vec3Df PhongBRDF::reflectance(const Vec3Df &incomingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	Vec3Df Lm = incomingVector;
	Vec3Df Nm = normal;
	Vec3Df V = reflectedVector;

	Vec3Df Rm = 2 * Vec3Df::dotProduct(Lm, Nm) * Nm - Lm;
	Rm.normalize();
	
	float VdotR = std::max<float>(0, Vec3Df::dotProduct(Rm, V));

	return light * this->material->sampleColor(texCoords) * std::pow(VdotR, this->material->getShininess());
}
