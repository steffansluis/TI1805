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
	* Initializes a point light.
	*/
	PointLight();

	/**
	* Initializes a point light with the given position.
	* @param position The position of the light.
	*/
	PointLight(const Vec3Df &position);

	/**
	 * Initializes a point light with the given position and color.
	 * @param position The position of the light.
	 * @param color The color of the light.
	 */
	PointLight(const Vec3Df &position, const Vec3Df &color);

	/**
	* Gets the color of the light.
	* @return The color of the light.
	*/
	Vec3Df getColor() const;

	/**
	* Gets the position of the light.
	* @return The position of the light.
	*/
	Vec3Df getPosition() const;

	/**
	* Sets the color of the light.
	* @param[in] color The color of the light.
	*/
	void setColor(const Vec3Df &color);

	/**
	* Sets the position of the light.
	* @param[in] position The position of the light.
	*/
	void setPosition(const Vec3Df &position);

	/**
	 * Calculates the direction from a point on the light source towards the given point
	 * and the light emitted along this direction.
	 * @param[in] point A point in the scene.
	 * @param[out] lightPoint A point on the light source.
	 * @param[out] lightColor The light emitted along the outgoing vector.
	 */
	void sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const;

private:
	Vec3Df color;
	Vec3Df position;
};

#endif