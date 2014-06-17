#include <algorithm>
#include <cassert>
#include <cmath>

#include "IMaterial.h"
#include "ITexture.h"
#include "OrenNayarBRDF.h"

OrenNayarBRDF::OrenNayarBRDF(const IMaterial *material)
: BRDF(material) {
	// Nothing to do here
}

// TODO: Implement the Oren-Nayar BRDF.
// Keep in mind the direction of the two vectors, you may have to flip a sign somewhere.
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#Formulation
// this->material->sampleDiffuseColor(texCoords) = specular color (Rho in the equations)
// this->material->sampleRoughness(texCoords) = roughness (Sigma in the equations)
// Tips:
// cos(theta_i) and cos(theta_r) can be computed using the dot product
// cos(phi_i - phi_r) can be compute by projecting the incoming and outgoing vectors onto the normal and then taking the dot product.

Vec3Df OrenNayarBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const {
	// TODO: oren-nayar brdf
	return this->material->sampleDiffuseColor(texCoords);
}