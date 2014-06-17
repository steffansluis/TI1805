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

// TODO: Implement the Oren-Nayar BRDF.
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#Formulation
// this->material->sampleDiffuseColor(texCoords) = specular color (Rho in the equations)
// this->material->sampleRoughness(texCoords) = roughness (Sigma in the equations)

// Tips:
// cos(theta_i) and cos(theta_r) can be computed using the dot product with the normal.
// cos(phi_i - phi_r) can be computed by projecting the incoming and reflected vectors onto the normal and then taking the dot product
// between the two projected vectors. (Vec3Df::projectOn with an empty vector for the second parameter).

// The incommingVector, reflectedVector and normal correspond to Li, Lr and n respectivly in this image
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#mediaviewer/File:Oren-nayar-reflection.png

// For now every object's material defaults to DiffuseMaterial, the DiffuseMaterial uses the LambertianBRDF by default.
// If you want to test a different brdf, modifiy the DiffuseMaterial(color, roughness) constructor to set diffuseBrdf to whichever one you need to test.
// DiffuseMaterial.cpp also contains the default color and roughness, change these as you wish.

Vec3Df OrenNayarBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	// TODO: oren-nayar brdf
	return this->material->sampleDiffuseColor(texCoords);
}