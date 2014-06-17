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
	* Calculates the amount of light that gets reflected from the incoming vector towards the reflected vector.
	* @param[in] incommingVector The vector in the direction that the light is coming from.
	* @param[in] reflectedVector The vector in the direction that the light is reflected to.
	* @param[in] normal The surface normal.
	* @param[in] texCoords The texture coordinates.
	* @param[in] light The light.
	* @return The amount of light reflected from the incomming towards the outgoing vector.
	*/
	virtual Vec3Df reflectance(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &normal, const Vec2Df &texCoords, const Vec3Df &light) const = 0;

protected:
	const IMaterial *material;
};

#endif