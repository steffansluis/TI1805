#ifndef ILIGHT_H
#define ILIGHT_H

#include <memory>

#include "Vec3D.h"

class IGeometry;

/**
 * Represents a light source.
 */
class ILight : public std::enable_shared_from_this<ILight> {
public:
	ILight();
	virtual ~ILight();

	/**
	* Gets the surface area of the light.
	* @return The surface area of the light.
	*/
	float getArea() const;
	
	/**
	 * Gets the geometry associated with this light, this can be null.
	 * @return The geometry associated with this light, this can be null.
	 */
	std::shared_ptr<IGeometry> getGeometry() const;

	/**
	 * Gets the falloff factor which determines how much the light dimishes over distance.
	 * The falloff from a point light source is quadratic.
	 * The range for this parameter is [0, 1], where 0 means no falloff and 1 means the amount of
	 * falloff is directly proportional to the quadratic distance.
	 * @return The falloff coefficient.
	 */
	float getFalloff() const;

	/**
	 * Gets the intensity of the light.
	 * The range for this parameter is [0, ->), where 0 means the light is completely black and
	 * values greater than 1 mean the light should cause some degree of bloom if a surface reflects
	 * the full intensity of the light.
	 * @return The intensity of the light.
	 */
	float getIntensity() const;

	/**
	 * Sets the geometry associated with this light, this can be null.
	 * @param[in] geometry The geometry associated with this light, this can be null.
	 */
	void setGeometry(std::shared_ptr<IGeometry> geometry);

	/**
	 * Sets the falloff factor which determines how much the light dimishes over distance.
	 * The falloff from a point light source is quadratic.
	 * The range for this parameter is [0, 1], where 0 means no falloff and 1 means the amount of
	 * falloff is directly proportional to the quadratic distance.
	 * @param falloff The falloff coefficient.
	 */
	void setFalloff(float falloff);

	/**
	 * Sets the intensity of the light.
	 * The range for this parameter is [0, ->), where 0 means the light is completely black and
	 * values greater than 1 mean the light should cause some degree of bloom if a surface reflects
	 * the full intensity of the light.
	 * @param intensity The intensity of the light.
	 */
	void setIntensity(float intensity);

	/**
	 * Perform any necessary preprocessing.
	 */
	virtual void preprocess();

	/**
	 * Calculates the direction from a point on the light source towards the given point
	 * and the light emitted along this direction.
	 * @param[in] point A point in the scene.
	 * @param[out] lightPoint A point on the light source.
	 * @param[out] lightColor The light emitted along the outgoing vector.
	 */
	virtual void sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const = 0;

protected:
	/**
	 * Calculates the amount of attenuation over distance from the falloff factor.
	 */
	float calculateAttenuation(float distance) const;

	/**
	 * Calculates the intensity of the light at the given distance with attenuation taken into account.
	 */
	float calculateIntensity(float distance) const;

	/**
	 * Calculates the amount of light 
	 */
	Vec3Df calculateLight(const Vec3Df &lightPoint, const Vec3Df &point);

private:
	float intensity;
	float falloff;
	std::shared_ptr<IGeometry> geometry;
};

#endif