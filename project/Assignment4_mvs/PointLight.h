#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "ILight.h"
#include "Vec3D.h"

/**
 * Implements a simple point light.
 */
class PointLight : public ILight {
public:
	/** 
	 * Initializes a point light with the given position and color.
	 * @return position The position of the light.
	 * @return color The color of the light.
	 */
	PointLight(const Vec3Df &position, const Vec3Df &color);

	/**
	* Calculates the direction from a point on the light source towards the given point
	* and returns the light emitted along this direction.
	* @param[in] point A point in the scene.
	* @param[out] outgoingVector A vector from the light source towards the given point.
	* @return The light emitted along the outgoing vector.
	*/
	Vec3Df getLightTowards(const Vec3Df &point, Vec3Df &outgoingVector);

private:
	Vec3Df position;
	Vec3Df color;
};

#endif