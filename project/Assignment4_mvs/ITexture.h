#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "Vec2D.h"
#include "Vec3D.h"

/**
 * Represents a texture.
 */
class ITexture {
public:
	virtual ~ITexture();

	/**
	 * Samples the texture at the given texture coordinates.
	 * @param uv The uv-coordinates.
	 * @return The color at the given texture coordinates.
	 */
	virtual Vec3Df sample(const Vec2Df &uv) const = 0;
};

#endif