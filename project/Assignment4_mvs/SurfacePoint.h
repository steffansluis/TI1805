#ifndef SURFACEPOINT_H
#define SURFACEPOINT_H

#include <memory>

#include "Vec2D.h"
#include "Vec3D.h"

class Scene;
class IGeometry;

/**
 * Represents a point at the surface of scene geometry.
 * Provides functions and data used for shading.
 */
class SurfacePoint {
public:
	/**
	 * Calculates the amount of ambient light hitting the surface.
	 * This is not physically correct.
	 * @param[in] scene The scene.
	 * @return The amount of ambient light hitting the surface.
	 */
	Vec3Df ambientLight(const Scene *scene) const;

	/**
	 * Calculates the light emitted from the surface towards the given vector.
	 * @param[in] reflectedVector The vector that the light is reflected towards.
	 * @return The light emitted from the surface towards the given vector.
	 */
	Vec3Df emittedLight(const Vec3Df &reflectedVector) const;

	/**
	 * Calculates the light reflected from the incoming vector towards the outgoing vector.
	 * @param[in] incommingVector The vector from which the light is comming.
	 * @param[in] reflectedVector The vector that the light is reflected towards.
	 * @param[in] lightColor The color of the light.
	 * @return The light reflected from the incomming towards the outgoing vector.
	 */
	Vec3Df reflectedLight(const Vec3Df &incommingVector, const Vec3Df &reflectedVector, const Vec3Df &lightColor) const;

	/**
	 * Calculates the specularly reflected light towards the given vector.
	 * This is not physically correct.
	 * @param[in] reflectedVector The vector that the light is reflected towards.
	 * @param[in] scene The scene.
	 * @return The light specularly reflected towards the outgoing vector.
	 */
	Vec3Df specularLight(const Vec3Df &reflectedVector, const Scene *scene) const;

	/**
	 * The point on the surface.
	 */
	Vec3Df point;

	/**
	 * The normal of the surface.
	 */
	Vec3Df normal;

	/**
	 * The texture coordinates of the surface.
	 */
	Vec2Df texCoords;

	/**
	 * The geometry which this surface belongs to.
	 */
	std::shared_ptr<const IGeometry> geometry;
};

#endif