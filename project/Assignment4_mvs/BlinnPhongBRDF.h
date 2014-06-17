#ifndef BLINNPHONGBRDF_H
#define BLINNPHONGBRDF_H

#include <memory>

#include "BRDF.h"

class ITexture;

/**
* Implements the Blinn-Phong reflectance model.
*/
class BlinnPhongBRDF : public BRDF {
public:
	/**
	* Initializes a Blinn-Phong brdf with the given texture.
	*/
	BlinnPhongBRDF(std::shared_ptr<const ITexture> specularTexture, float shininess);

	/**
	* Calculates the amount of light reflected from the incoming vector towards the outgoing vector.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] normal The surface normal.
	* @param[in] texCoords The texture coordinates.
	* @return The amount of light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const;

private:
	std::shared_ptr<const ITexture> specularTexture;
	float shininess;
};

#endif