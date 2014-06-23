#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <memory>

#include "Vec2D.h"
#include "Vec3D.h"

class BRDF;
class ITexture;
class Scene;
class SurfacePoint;

/**
 * Represents a material.
 * @remarks Needs work to make this class pratical, generate from .mtl material.
 */
class IMaterial : public std::enable_shared_from_this<IMaterial> {
public:
	IMaterial();
	virtual ~IMaterial();

	/**
	 * Gets the texture of this material.
	 */
	std::shared_ptr<const ITexture> getTexture() const;
	/**
	 * Gets the amount of ambient light this material reflects.
	 * This is a scalar in the range [0, 1].
	 */
	float getAmbientReflectance() const;
	/**
	 * Gets the amount of diffuse light this material reflects.
	 * This is a scalar in the range [0, 1].
	 */
	float getDiffuseReflectance() const;
	/**
	 * Gets the amount of specular light this material reflects.
	 * This is a scalar in the range [0, 1].
	 */
	float getSpecularReflectance() const;
	/**
	 * Gets the amount of light this material emits.
	 * This is a scalar in the range [0, ∞).
	 */
	float getEmissiveness() const;
	/**
	 * Gets the amount of light that can pass through this material.
	 * This is a scalar in the range [0, 1].
	 */
	float getTransparency() const;
	/**
	 * Gets the absorbance of this material in the Beer-Lambert law.
	 * This is a scalar in the range [0, ∞).
	 */
	float getAbsorbance() const;
	/**
	 * Gets the roughness of this material.
	 * This is a scalar in the range [0, 1].
	 */
	float getRoughness() const;
	/**
	 * Gets the shininess of this material.
	 * This is a scalar in the range [0, ∞).
	 */
	float getShininess() const;
	/**
	 * Gets the refractive index of this material.
	 * This is a scalar in the range [0, ∞).
	 */
	float getRefractiveIndex() const;

	/**
	 * Sets the texture of this material.
	 */
	void setTexture(std::shared_ptr<const ITexture>);
	/**
	 * Sets the amount of ambient light this material reflects.
	 * This is a scalar in the range [0, 1].
	 */
	void setAmbientReflectance(float ambientReflectance);
	/**
	 * Sets the amount of diffuse light this material reflects.
	 * This is a scalar in the range [0, 1].
	 */
	void setDiffuseReflectance(float diffuseReflectance);
	/**
	 * Sets the amount of specular light this material reflects.
	 * This is a scalar in the range [0, 1].
	 */
	void setSpecularReflectance(float specularReflectance);
	/**
	 * Sets the amount of light this material emits.
	 * This is a scalar in the range [0, ∞).
	 */
	void setEmissiveness(float emissiveness);
	/**
	 * Sets the amount of light that can pass through this material.
	 * This is a scalar in the range [0, 1].
	 */
	void setTransparency(float transparency);
	/**
	 * Sets the absorbance of this material in the Beer-Lambert law.
	 * This is a scalar in the range [0, ∞).
	 */
	void setAbsorbance(float absorbance);
	/**
	 * Sets the roughness of this material.
	 * This is a scalar in the range [0, 1].
	 */
	void setRoughness(float roughness);
	/**
	 * Sets the shininess of this material.
	 * This is a scalar in the range [0, ∞).
	 */
	void setShininess(float shininess);
	/**
	 * Sets the refractive index of this material.
	 * This is a scalar in the range [0, ∞).
	 */
	void setRefractiveIndex(float refractiveIndex);

	/**
	 * Samples the color of the material at the given texture coordinates.
	 */
	Vec3Df sampleColor(const Vec2Df &texCoords) const;

	/**
	 * Sets the diffuse BRDF of this material using its type.
	 * Example usage: material.setBRDF<LambertianBRDF>().
	 */
	template<class T>
	void setDiffuseBRDF();

	/**
	* Sets the specular BRDF of this material using its type.
	* Example usage: material.setBRDF<LambertianBRDF>().
	*/
	template<class T>
	void setSpecularBRDF();

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
	Vec3Df reflectedLight(const SurfacePoint &surface, const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &lightColor) const;

	/**
	 * Calculates the specularly reflected light towards the given vector.
	 * @param[in] surface The surface for which to perform the calculations.
	 * @param[in] reflectedVector The vector that the light is reflected towards.
	 * @param[in] scene The scene.
	 * @param[in] iteration	The current iteration.
	 * @return The light specularly reflected towards the outgoing vector.
	 */
	Vec3Df specularLight(
		const SurfacePoint &surface,
		const Vec3Df &reflectedVector, 
		const Scene *scene,
		int iteration) const;

	/**
	* Calculates the transmitted light towards the given vector.
	* @param[in] surface The surface for which to perform the calculations.
	* @param[in] reflectedVector The vector that the light is reflected towards.
	* @param[in] scene The scene.
	* @param[in] iteration	The current iteration.
	* @return The light specularly reflected towards the outgoing vector.
	*/
	Vec3Df transmittedLight(
		const SurfacePoint &surface,
		const Vec3Df &reflectedVector,
		const Scene *scene,
		int iteration) const;

private:
	/**
	* Calculates the reflection vector.
	* @param[in] incommingVector The vector in the direction that the light is coming from.
	* @param[in] normal The surface normal.
	*/
	static Vec3Df calculateReflectionVector(
		const Vec3Df &incommingVector,
		const Vec3Df &normal);

	/**
	* Calculates the refracted vector.
	* @param[in] incommingVector The vector in the direction that the light is coming from.
	* @param[in] normal The surface normal.
	* @param[in] n1 Refractive index of the medium the incommingVector is coming from.
	* @param[in] n2 Refractive index of the surface.
	*/
	static Vec3Df calculateRefractedVector(
		const Vec3Df &incommingVector,
		const Vec3Df &normal,
		float n1,
		float n2);

	std::shared_ptr<const ITexture> texture;
	float ambientReflectance;
	float diffuseReflectance;
	float specularReflectance;
	float emissiveness;
	float transparency;
	float absorbance;
	float roughness;
	float shininess;
	float refractiveIndex;
	const BRDF *diffuseBrdf;
	const BRDF *specularBrdf;
};

#endif