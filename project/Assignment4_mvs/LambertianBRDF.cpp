#include <algorithm>
#include <cassert>
#include <cmath>

#include "Constants.h"
#include "IMaterial.h"
#include "ITexture.h"
#include "LambertianBRDF.h"

LambertianBRDF::LambertianBRDF(const IMaterial *material)
: BRDF(material) {
	// Nothing to do here
}

Vec3Df LambertianBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	Vec3Df rho = this->material->sampleColor(texCoords);
	float cosThetaI = std::max(0.0f, Vec3Df::dotProduct(incommingVector, normal));

	Vec3Df Lr = (rho / Constants::Pi) * cosThetaI * light;
	return Lr;
}