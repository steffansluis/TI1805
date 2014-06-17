#include <algorithm>
#include <cassert>
#include <cmath>

#include "BlinnPhongBRDF.h"
#include "IMaterial.h"
#include "ITexture.h"

BlinnPhongBRDF::BlinnPhongBRDF(const IMaterial *material)
: BRDF(material) {
	// Nothing to do here
}

// TODO: Implement the Blinn-Phong BRDF.
// Keep in mind the direction of the two vectors, you may have to flip a sign somewhere.
// Ignore the diffuse part.
// http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model
// this->material->sampleSpecularColor(texCoords) = specular color
// this->material->sampleShininess(texCoords) = shininess

Vec3Df BlinnPhongBRDF::evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const {
	// TODO: Blinn-Phong brdf
	return this->material->sampleSpecularColor(texCoords);
}