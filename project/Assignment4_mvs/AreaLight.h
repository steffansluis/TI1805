#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "ILight.h"

class AreaLight : public ILight {
public:
	/**
	 * Initializes a point light with the given position and color.
	 * @param position The position of the light.
	 * @param color The color of the light.
	 */
	AreaLight(std::shared_ptr<IGeometry> geometry);

	/**
	 * Calculates the direction from a point on the light source towards the given point
	 * and the light emitted along this direction.
	 * @param[in] point A point in the scene.
	 * @param[out] lightPoint A point on the light source.
	 * @param[out] lightColor The light emitted along the outgoing vector.
	 * @return Returns true if the point is visible from the light source; otherwise false.
	 */
	bool sampleLight(const Vec3Df &point, Vec3Df &lightPoint, Vec3Df &lightColor) const;
};

#endif