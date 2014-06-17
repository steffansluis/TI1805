#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "ITexture.h"

/**
 * Represents a texture.
 */
class Texture : public ITexture {
public:
	/**
	 * Loads the texture with the given filename.
	 */
	Texture(std::string textureName);

	/**
	 * Samples the texture at the given texture coordinates.
	 * @param uv The uv-coordinates.
	 * @return The color at the given texture coordinates.
	 */
	Vec3Df sample(const Vec2Df &uv) const;
};

#endif