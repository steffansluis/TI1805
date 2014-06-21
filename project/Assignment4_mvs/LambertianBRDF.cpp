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
// http://en.wikipedia.org/wiki/Lambertian_reflectance
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#Formulation
// this->material->sampleColor(texCoords) * this->material->getDiffuseCoefficient() = diffuse color

// The incommingVector, reflectedVector and normal correspond to Li, Lr and n respectivly in this image
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#mediaviewer/File:Oren-nayar-reflection.png

// For now every object's material defaults to DiffuseMaterial, the DiffuseMaterial uses the LambertianBRDF by default.
// If you want to test a different brdf, modifiy the DiffuseMaterial(color, roughness) constructor to set diffuseBrdf to whichever one you need to test.
// DiffuseMaterial.cpp also contains the default color and roughness, change these as you wish.

Vec3Df LambertianBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	return this->material->sampleColor(texCoords) * this->material->getDiffuseCoefficient() * std::max<float>(0, Vec3Df::dotProduct(incommingVector, normal)) * light;
}