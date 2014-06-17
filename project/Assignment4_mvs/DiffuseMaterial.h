#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "IMaterial.h"

class BRDF;

class DiffuseMaterial : public IMaterial {
public:
	DiffuseMaterial();
	DiffuseMaterial(const Vec3Df &color);
	DiffuseMaterial(const Vec3Df &color, float roughness);
	~DiffuseMaterial();

	/**
	* Calculates the light reflected from the incoming vector towards the outgoing vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] lightColor The color of the light.
	* @return The light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df reflectedLight(const SurfacePoint &surface, const Vec3Df &incommingVector, const Vec3Df &outgoingVector, const Vec3Df &lightColor) const;

	/**
	* Calculates the specularly reflected light towards the given vector.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] scene The scene.
	* @return The light specularly reflected towards the outgoing vector.
	*/
	Vec3Df specularLight(const SurfacePoint &surface, const Vec3Df &outgoingVector, const Scene *scene) const;

private:
	std::shared_ptr<const BRDF> diffuseBrdf;
};

#endif