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

// TODO: Implement the lambertian BRDF, this should be pretty much identical to your diffuseOnly sharer for assignment 3.
// Keep in mind the direction of the two vectors, you may have to flip a sign somewhere.
// http://en.wikipedia.org/wiki/Lambertian_reflectance
// this->material->sampleDiffuseColor(texCoords) = diffuse color

// It is irrelevant which direction is towards the light source and which direction is towards the observer due to helmholz reciprocity.
// http://en.wikipedia.org/wiki/Helmholtz_reciprocity

// For now every object's material defaults to DiffuseMaterial, the DiffuseMaterial uses the LambertianBRDF by default.
// If you want to test a different brdf, modifiy the DiffuseMaterial(color, roughness) constructor to set diffuseBrdf to whichever one you need to test.
// DiffuseMaterial.cpp also contains the default color and roughness, change these as you wish.

Vec3Df LambertianBRDF::evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const {
	// TODO: Diffuse shading
	return this->material->sampleDiffuseColor(texCoords);
}