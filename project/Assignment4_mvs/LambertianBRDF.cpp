#include <algorithm>
#include <cassert>
#include <cmath>

#include "ITexture.h"
#include "LambertianBRDF.h"

LambertianBRDF::LambertianBRDF(std::shared_ptr<const ITexture> diffuseTexture)
: diffuseTexture(diffuseTexture) {
	assert(diffuseTexture);

	// Nothing to do here
}

// TODO: Implement the lambertian BRDF, this should be pretty much identical to your diffuseOnly sharer for assignment 3.
// Keep in mind the direction of the two vectors, you may have to flip a sign somewhere.
// http://en.wikipedia.org/wiki/Lambertian_reflectance

// It is irrelevant which direction is towards the light source and which direction is towards the observer due to helmholz reciprocity.
// http://en.wikipedia.org/wiki/Helmholtz_reciprocity

Vec3Df LambertianBRDF::evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const {
	// TODO: Diffuse shading
	return this->diffuseTexture->sample(texCoords);
}