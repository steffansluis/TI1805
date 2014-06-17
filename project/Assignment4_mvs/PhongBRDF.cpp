#include <algorithm>
#include <cassert>
#include <cmath>

#include "IMaterial.h"
#include "ITexture.h"
#include "PhongBRDF.h"

PhongBRDF::PhongBRDF(const IMaterial * material)
: BRDF(material) {
	// Nothing to do here
}

// TODO: Implement the Phong BRDF.
// You can ignore the diffuse part.
// http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model
// this->material->sampleSpecularColor(texCoords) = specular color
// this->material->sampleShininess(texCoords) = shininess

// The incommingVector, reflectedVector and normal correspond to Li, Lr and n respectivly in this image
// http://en.wikipedia.org/wiki/Oren%E2%80%93Nayar_reflectance_model#mediaviewer/File:Oren-nayar-reflection.png

// For now every object's material defaults to DiffuseMaterial, the DiffuseMaterial uses the LambertianBRDF by default.
// If you want to test a different brdf, modifiy the DiffuseMaterial(color, roughness) constructor to set diffuseBrdf to whichever one you need to test.
// DiffuseMaterial.cpp also contains the default color and roughness, change these as you wish.

Vec3Df PhongBRDF::reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const {
	// TODO: Phong brdf
	return this->material->sampleSpecularColor(texCoords);
}