#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <memory>

#include "Vec3D.h"

class ITexture;
class SurfacePoint;

/**
 * Represents a material.
 * @remarks Needs work to make this class pratical, generate from .mtl material.
 */
class IMaterial : public std::enable_shared_from_this<IMaterial> {
public:
	IMaterial();
	virtual ~IMaterial();

	std::shared_ptr<const ITexture> getAmbientTexture() const;
	std::shared_ptr<const ITexture> getDiffuseTexture() const;
	std::shared_ptr<const ITexture> getEmissiveTexture() const;
	std::shared_ptr<const ITexture> getSpecularTexture() const;

	void setAmbientTexture(std::shared_ptr<const ITexture> texture);
	void setDiffuseTexture(std::shared_ptr<const ITexture> texture);
	void setEmissiveTexture(std::shared_ptr<const ITexture> texture);
	void setSpecularTexture(std::shared_ptr<const ITexture> texture);

	/**
	* Calculates the amount of ambient light hitting the surface.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @return The amount of ambient light hitting the surface.
	*/
	Vec3Df ambientLight(std::shared_ptr<const SurfacePoint> surface) const;

	/**
	* Calculates the light emitted from the surface towards the given vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @return The light emitted from the surface towards the given vector.
	*/
	Vec3Df emittedLight(std::shared_ptr<const SurfacePoint>, const Vec3Df &outgoingVector) const;

	/**
	* Calculates the light reflected from the incoming vector towards the outgoing vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] lightColor The color of the light.
	* @return The light reflected from the incomming towards the outgoing vector.
	*/
	virtual Vec3Df reflectedLight(std::shared_ptr<const SurfacePoint>, const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const = 0;

	/**
	* Calculates the specularly reflected light towards the given vector.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @return The light specularly reflected towards the outgoing vector.
	*/
	virtual Vec3Df specularLight(std::shared_ptr<const SurfacePoint>, const Vec3Df &outgoingVector) const = 0;

private:
	std::shared_ptr<const ITexture> ambientTexture;
	std::shared_ptr<const ITexture> diffuseTexture;
	std::shared_ptr<const ITexture> emissiveTexture;
	std::shared_ptr<const ITexture> specularTexture;
};

#endif