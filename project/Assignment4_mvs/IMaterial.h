#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "Vec3D.h"

class ITexture;
class SurfacePoint;

/**
 * Represents a material.
 * @remarks Needs work to make this class pratical, generate from .mtl material.
 */
class IMaterial {
public:
	virtual ~IMaterial();

	const ITexture *getAmbientTexture() const;
	const ITexture *getDiffuseTexture() const;
	const ITexture *getEmissiveTexture() const;
	const ITexture *getSpecularTexture() const;

	const ITexture *setAmbientTexture(const ITexture *texture);
	const ITexture *setDiffuseTexture(const ITexture *texture);
	const ITexture *setEmissiveTexture(const ITexture *texture);
	const ITexture *setSpecularTexture(const ITexture *texture);

	/**
	* Calculates the amount of ambient light hitting the surface.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @return The amount of ambient light hitting the surface.
	*/
	Vec3Df ambientLight(const SurfacePoint *surface) const;

	/**
	* Calculates the light emitted from the surface towards the given vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @return The light emitted from the surface towards the given vector.
	*/
	Vec3Df emittedLight(const SurfacePoint *surface, const Vec3Df &outgoingVector) const;

	/**
	* Calculates the light reflected from the incoming vector towards the outgoing vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] lightColor The color of the light.
	* @return The light reflected from the incomming towards the outgoing vector.
	*/
	virtual Vec3Df reflectedLight(const SurfacePoint *surface, const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const = 0;

	/**
	* Calculates the specularly reflected light towards the given vector.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @return The light specularly reflected towards the outgoing vector.
	*/
	virtual Vec3Df specularLight(const SurfacePoint *surface, const Vec3Df &outgoingVector) const = 0;

private:
	const ITexture *ambientTexture;
	const ITexture *diffuseTexture;
	const ITexture *emissiveTexture;
	const ITexture *specularTexture;
};

#endif