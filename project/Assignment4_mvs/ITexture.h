#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "Vec3D.h"

/**
 * Represents a texture.
 */
class ITexture {
public:
	virtual ~ITexture();

	/**
	 * Samples the texture at the given texture coordinates.
	 * @param u The u-coordinate.
	 * @param v The v-coordinate.
	 * @return The color at the given texture coordinates.
	 */
	virtual Vec3Df sample(float u, float v) const = 0;
};

#endif