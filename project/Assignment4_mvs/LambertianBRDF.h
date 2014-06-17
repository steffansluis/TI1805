#ifndef LAMBERTIANBRDF_H
#define LAMBERTIANBRDF_H

#include <memory>

#include "BRDF.h"

class IMatertial;

/**
 * Implements the lambertian reflectance model.
 */
class LambertianBRDF : public BRDF {
public:
	/**
	 * Initializes a lambertian brdf with the given material.
	 */
	LambertianBRDF(const IMaterial *material);

	/**
	* Calculates the amount of light reflected from the incoming vector towards the outgoing vector.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] normal The surface normal.
	* @param[in] texCoords The texture coordinates.
	* @return The amount of light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df reflectance(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const;
};

#endif