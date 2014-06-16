#ifndef CONSTANTTEXTURE_H
#define CONSTANTTEXTURE_H

#include "ITexture.h"

/**
 * Implements a texture returning a constant value.
 */
class ConstantTexture : public ITexture {
public:
	/**
	 * Initializes a constant texture with the given color.
	 */
	ConstantTexture(const Vec3Df &color);
	
	/**
	* Samples the texture at the given texture coordinates.
	* @param u The u-coordinate.
	* @param v The v-coordinate.
	* @return The color at the given texture coordinates.
	*/
	Vec3Df sample(float u, float v) const;

private:
	Vec3Df color;
};

#endif