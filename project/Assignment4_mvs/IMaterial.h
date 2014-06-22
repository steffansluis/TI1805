#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <memory>

#include "Vec2D.h"
#include "Vec3D.h"

class BRDF;
class ITexture;
class Reflection;
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

	std::shared_ptr<const ITexture> getTexture() const;
	float getAmbientCoefficient() const;
	float getDiffuseCoefficient() const;
	float getEmissiveness() const;
	float getReflectiveness() const;
	float getTransparency() const;
	float getAbsorbance() const;
	float getRoughness() const;
	float getShininess() const;
	float getRefractiveIndex() const;

	void setTexture(std::shared_ptr<const ITexture>);
	void setAmbientCoefficient(float ambientCoefficient);
	void setDiffuseCoefficient(float diffuseCoefficient);
	void setEmissiveness(float emissiveness);
	void setReflectiveness(float reflectiveness);
	void setTransparency(float transparency);
	void setAbsorbance(float absorbance);
	void setRoughness(float roughness);
	void setShininess(float shininess);
	void setRefractiveIndex(float refractiveIndex);

	Vec3Df sampleColor(const Vec2Df &texCoords) const;

	template<class T>
	void setBRDF();

	template<class T>
	void setReflection();

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
	 * This is not physically correct.
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

private:
	std::shared_ptr<const ITexture> texture;
	float ambientCoefficient;
	float diffuseCoefficient;
	float emissiveness;
	float reflectiveness;
	float transparency;
	float absorbance;
	float roughness;
	float shininess;
	float refractiveIndex;
	const BRDF *brdf;
	const Reflection *reflection;
};

#endif