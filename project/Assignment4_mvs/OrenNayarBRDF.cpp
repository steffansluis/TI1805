#include <algorithm>
#include <cassert>
#include <cmath>

#include "ITexture.h"
#include "OrenNayarBRDF.h"

OrenNayarBRDF::OrenNayarBRDF(std::shared_ptr<const ITexture> diffuseTexture, float roughness)
: diffuseTexture(diffuseTexture), roughness(roughness) {
	assert(diffuseTexture);

	// Nothing to do here
}

// TODO: Implement the Oren-Nayar BRDF.
// Keep in mind the direction of the two vectors, you may have to flip a sign somewhere.
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#Formulation
// this->diffuseTexture->sample(texCoords) = Rho in the equation
// this->roughness = Sigma in the equations.

Vec3Df OrenNayarBRDF::evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const {
	// TODO: oren-nayar brdf
	return this->diffuseTexture->sample(texCoords);
}