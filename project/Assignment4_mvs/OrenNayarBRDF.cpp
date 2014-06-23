#define _USE_MATH_DEFINES

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

/**
	The Oren Nayar BRDF.
	Implemented using the formulaes described in http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#Formulation

	@author Joren Hammudoglu
*/
Vec3Df OrenNayarBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	Vec3Df empty = Vec3Df(0,0,0);
	Vec3Df rho =  this->material->sampleColor(texCoords);
	float sigma = this->material->getRoughness();
	float sigma2 = sigma*sigma;
	float cosThetaI = Vec3Df::dotProduct(incommingVector,normal);
	float cosThetaR = Vec3Df::dotProduct(reflectedVector,normal);
	float cosPhiDiff = Vec3Df::dotProduct(
			incommingVector.projectOn(normal,empty),
			reflectedVector.projectOn(normal,empty)
	);

	float A = 1 - 0.5 * (sigma2 / (sigma2 + 0.33));
	float B = 0.45 * (sigma2/ (sigma2 + 0.09));

	float alhpa = std::max(cosThetaI, cosThetaR);
	float beta = std::min(cosThetaI, cosThetaR);

	Vec3Df Lr = (rho/M_PI) * cosThetaI * (A + (B * std::max(0.f, cosPhiDiff) * std::sin(alhpa) * std::tan(beta))) * light;
	return Lr;
}
