#ifndef ILIGHT_H
#define ILIGHT_H

#include "Vec3D.h"

/**
 * Represents a light source.
 */
class ILight {
public:
	virtual ~ILight();

	/**
	* Perform any necessary preprocessing.
	*/
	virtual void preprocess();

	/**
	 * Calculates the direction from a point on the light source towards the given point
	 * and returns the light emitted along this direction.
	 * @param[in] point A point in the scene.
	 * @param[out] outgoingVector A vector from the light source towards the given point.
	 * @return The light emitted along the outgoing vector.
	 */
	virtual Vec3Df getLightTowards(const Vec3Df &point, Vec3Df &outgoingVector) = 0;
};

#endif