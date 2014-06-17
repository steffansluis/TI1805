#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <memory>

#include "Vec2D.h"
#include "Vec3D.h"

class Scene;
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

	std::shared_ptr<const ITexture> getAmbientColor() const;
	std::shared_ptr<const ITexture> getDiffuseColor() const;
	std::shared_ptr<const ITexture> getEmissiveColor() const;
	std::shared_ptr<const ITexture> getSpecularColor() const;
	std::shared_ptr<const ITexture> getRoughness() const;
	std::shared_ptr<const ITexture> getShininess() const;

	void setAmbientColor(std::shared_ptr<const ITexture> texture);
	void setDiffuseColor(std::shared_ptr<const ITexture> texture);
	void setEmissiveColor(std::shared_ptr<const ITexture> texture);
	void setSpecularColor(std::shared_ptr<const ITexture> texture);
	void setRoughness(std::shared_ptr<const ITexture> roughness);
	void setShininess(std::shared_ptr<const ITexture> shininess);

	Vec3Df sampleAmbientColor(const Vec2Df &texCoords) const;
	Vec3Df sampleDiffuseColor(const Vec2Df &texCoords) const;
	Vec3Df sampleEmissiveColor(const Vec2Df &texCoords) const;
	Vec3Df sampleSpecularColor(const Vec2Df &texCoords) const;
	float sampleRoughness(const Vec2Df &texCoords) const;
	float sampleShininess(const Vec2Df &texCoords) const;

	/**
	* Calculates the amount of ambient light hitting the surface.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] scene The scene.
	* @return The amount of ambient light hitting the surface.
	*/
	Vec3Df ambientLight(const SurfacePoint &surface, const Scene *scene) const;

	/**
	* Calculates the light emitted from the surface towards the given vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] reflectedVector The vector that the light is reflected towards.
	* @return The light emitted from the surface towards the given vector.
	*/
	Vec3Df emittedLight(const SurfacePoint &surface, const Vec3Df &reflectedVector) const;

	/**
	* Calculates the light reflected from the incoming vector towards the outgoing vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] reflectedVector The vector that the light is reflected towards.
	* @param[in] lightColor The color of the light.
	* @return The light reflected from the incomming towards the outgoing vector.
	*/
	virtual Vec3Df reflectedLight(const SurfacePoint &surface, const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &lightColor) const = 0;

	/**
	* Calculates the specularly reflected light towards the given vector.
	* This is not physically correct.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] reflectedVector The vector that the light is reflected towards.
	* @param[in] scene The scene.
	* @return The light specularly reflected towards the outgoing vector.
	*/
	virtual Vec3Df specularLight(const SurfacePoint &surface, const Vec3Df &reflectedVector, const Scene *scene) const = 0;

private:
	std::shared_ptr<const ITexture> ambientTexture;
	std::shared_ptr<const ITexture> diffuseTexture;
	std::shared_ptr<const ITexture> emissiveTexture;
	std::shared_ptr<const ITexture> specularTexture;
	std::shared_ptr<const ITexture> roughness;
	std::shared_ptr<const ITexture> shininess;
};

#endif