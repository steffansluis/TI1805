#ifndef ORENNAYARBRDF_H
#define ORENNAYARBRDF_H

#include <memory>

#include "BRDF.h"

class IMaterial;

/**
* Implements the Oren-Nayar reflectance model.
*/
class OrenNayarBRDF : public BRDF {
public:
	/**
	* Initializes a Oren-Nayar brdf with the given material.
	*/
	OrenNayarBRDF(const IMaterial *material);

	/**
	* Calculates the amount of light reflected from the incoming vector towards the outgoing vector.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] normal The surface normal.
	* @param[in] texCoords The texture coordinates.
	* @return The amount of light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df evaluate(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const;
};

#endif