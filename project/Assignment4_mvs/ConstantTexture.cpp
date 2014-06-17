#include "ConstantTexture.h"

ConstantTexture::ConstantTexture(const Vec3Df &color) {
	// Set the texture's color constant
	this->color = color;
}

Vec3Df ConstantTexture::sample(const Vec2Df &uv) const {
	// Returns the texture's color constant
	return this->color;
}