#include <algorithm>
#include <cassert>
#include <cmath>

#include "Constants.h"
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

	// Get the cosines of the angles between (I and N) and (R and N)
	float cosThetaI = std::max(0.0f, Vec3Df::dotProduct(incommingVector,normal));
	float cosThetaR = std::max(0.0f, Vec3Df::dotProduct(reflectedVector, normal));

	// Project I and R onto the surface and normalize them
	Vec3Df incommingLocal = incommingVector.projectOn(normal, empty);
	Vec3Df reflectedLocal = reflectedVector.projectOn(normal, empty);
	incommingLocal.normalize();
	reflectedLocal.normalize();

	// Get the cosine of the angle between I and R
	float cosPhiDiff = Vec3Df::dotProduct(incommingLocal, reflectedLocal);

	float A = 1 - 0.5f * (sigma2 / (sigma2 + 0.33f));
	float B = 0.45f * (sigma2/ (sigma2 + 0.09f));

	// Get the actual angles
	float thetaI = std::acos(cosThetaI);
	float thetaR = std::acos(cosThetaR);

	float alhpa = std::max(thetaI, thetaR);
	float beta = std::min(thetaI, thetaR);

	Vec3Df Lr = rho * cosThetaI * (A + (B * std::max(0.f, cosPhiDiff) * std::sin(alhpa) * std::tan(beta))) * light;
	return Lr;
}
