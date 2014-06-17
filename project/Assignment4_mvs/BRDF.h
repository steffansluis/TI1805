#ifndef BRDF_H
#define BRDF_H

#include <memory>

#include "Vec2D.h"
#include "Vec3D.h"

class IMaterial;

/**
 * Represents a bidirectional reflectance distribution function.
 */
class BRDF {
public:
	BRDF(const IMaterial *material);

	/**
	* Calculates the amount of light reflected from the incoming vector towards the outgoing vector.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] normal The surface normal.
	* @param[in] texCoords The texture coordinates.
	* @return The amount of light reflected from the incomming towards the outgoing vector.
	*/
	virtual Vec3Df reflectance(const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &normal, const Vec2Df &texCoords) const = 0;

protected:
	const IMaterial *material;
};

#endif