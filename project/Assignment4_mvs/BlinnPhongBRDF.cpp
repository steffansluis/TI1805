#include <algorithm>
#include <cassert>
#include <cmath>

#include "ITexture.h"
#include "BlinnPhongBRDF.h"

BlinnPhongBRDF::BlinnPhongBRDF(std::shared_ptr<const ITexture> specularTexture, float shininess)
: specularTexture(specularTexture), shininess(shininess) {
	assert(specularTexture);

	// Nothing to do here
}

// TODO: Implement the Blinn-Phong BRDF.
// Keep in mind the direction of the two vectors, you may have to flip a sign somewhere.
// Ignore the diffuse part.
// http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model

Vec3Df BlinnPhongBRDF::evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const {
	// TODO: Blinn-Phong brdf
	return this->specularTexture->sample(texCoords);
}