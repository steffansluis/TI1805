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
// You can ignore the diffuse part.
// http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model
// this->material->sampleSpecularColor(texCoords) = specular color
// this->material->sampleShininess(texCoords) = shininess

// The incommingVector, reflectedVector and normal correspond to Li, Lr and n respectivly in this image
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#mediaviewer/File:Oren-nayar-reflection.png

Vec3Df BlinnPhongBRDF::reflectance(const Vec3Df &incomingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	Vec3Df H = incomingVector + reflectedVector;
	H.normalize();

	Vec3Df N = normal;
	N.normalize();

	float NdotH = std::max<float>(0, Vec3Df::dotProduct(N, H));
	return light * this->material->sampleColor(texCoords) * std::pow(NdotH, this->material->getShininess());
}
