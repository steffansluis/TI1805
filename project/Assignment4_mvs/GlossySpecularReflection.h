#ifndef GLOSSYSPECULARREFLECTION_H
#define GLOSSYSPECULARREFLECTION_H

#include "Reflection.h"

class GlossySpecularReflection : public Reflection {
public:
	GlossySpecularReflection(const IMaterial *material);

	/**
	 * Calculates the reflection vector, reflectance, refraction vector and transmittance.
	 * @param[in] incommingVector The vector in the direction that the light is coming from.
	 * @param[in] normal The surface normal.
	 * @param[in] texCoords The texture coordinates.
	 * @param[in] n1 Refractive index of the medium the incommingVector is coming from.
	 * @param[in] n2 Refractive index of the surface.
	 * @param[out] reflectedVector The reflected vector.
	 * @param[out] reflectedVector The reflected vector.
	 * @return The amount of light reflected from the incomming towards the outgoing vector.
	 */
	void sample(
		const Vec3Df &incommingVector,
		const Vec3Df &normal,
		const Vec2Df &texCoords,
		float n1,
		float n2,
		Vec3Df &reflectedVector,
		Vec3Df &reflectedColor,
		Vec3Df &refractedVector,
		Vec3Df &refractedColor) const;
};

#endif