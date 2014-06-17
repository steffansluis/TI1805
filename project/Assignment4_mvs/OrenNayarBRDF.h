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
	* Calculates the amount of light that gets reflected from the incoming vector towards the reflected vector.
	* @param[in] incommingVector The vector in the direction that the light is coming from.
	* @param[in] reflectedVector The vector in the direction that the light is reflected to.
	* @param[in] normal The surface normal.
	* @param[in] texCoords The texture coordinates.
	* @param[in] light The light.
	* @return The amount of light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const;
};

#endif