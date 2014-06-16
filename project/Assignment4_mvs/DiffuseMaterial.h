#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "IMaterial.h"

class DiffuseMaterial : public IMaterial {
public:
	DiffuseMaterial();
	DiffuseMaterial(const Vec3Df &color);
	~DiffuseMaterial();

	/**
	* Calculates the light reflected from the incoming vector towards the outgoing vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] lightColor The color of the light.
	* @return The light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df reflectedLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const;

	/**
	* Calculates the specularly reflected light towards the given vector.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @return The light specularly reflected towards the outgoing vector.
	*/
	Vec3Df specularLight(std::shared_ptr<const SurfacePoint> surface, const Vec3Df &outgoingVector) const;
};

#endif