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
	* @param position The position of the light.
	* @param color The color of the light.
	*/
	PointLight(const Vec3Df &position, const Vec3Df &color);

	/**
	* Calculates the direction from a point on the light source towards the given point
	* and the light emitted along this direction.
	* @param[in] point A point in the scene.
	* @param[out] lightPoint A point on the light source.
	* @param[out] lightColor The light emitted along the outgoing vector.
	*/
	void sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const;

private:
	Vec3Df position;
	Vec3Df color;
};

#endif