#ifndef REFLECTION_H
#define REFLECTION_H

#include "Vec2D.h"
#include "Vec3D.h"

class IMaterial;

class Reflection {
public:
	Reflection(const IMaterial *material);
	
	/**
	 * Calculates the reflection vector, reflectance, refraction vector and transmittance.
	 * @param[in] incommingVector The vector in the direction that the light is coming from.
	 * @param[in] normal The surface normal.
	 * @param[in] texCoords The texture coordinates.
	 * @param[out] reflectedVector The reflected vector.
	 * @param[out] reflectance The reflectance.
	 * @param[out] refractedVector The reflected vector.
	 * @param[out] transmittance The transmittance.
	 */
	virtual void sample(
		const Vec3Df &incommingVector, 
		const Vec3Df &normal, 
		const Vec2Df &texCoords, 
		Vec3Df &reflectedVector,
		Vec3Df &reflectance,
		Vec3Df &refractedVector,
		Vec3Df &transmittance) const = 0;

protected:
	const IMaterial *material;
};

#endif