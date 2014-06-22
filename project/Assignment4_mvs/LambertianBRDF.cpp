#include <algorithm>
#include <cassert>
#include <cmath>

#include "IMaterial.h"
#include "ITexture.h"
#include "LambertianBRDF.h"

LambertianBRDF::LambertianBRDF(const IMaterial *material)
: BRDF(material) {
	// Nothing to do here
}

// The incommingVector, reflectedVector and normal correspond to Li, Lr and n respectivly in this image
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#mediaviewer/File:Oren-nayar-reflection.png

Vec3Df LambertianBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	float LdotN = std::max<float>(0, Vec3Df::dotProduct(incommingVector, normal));

	return light * this->material->sampleColor(texCoords) * LdotN;
}