#ifndef SURFACEPOINT_H
#define SURFACEPOINT_H

#include "Vec3D.h"

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
	 * @return The amount of ambient light hitting the surface.
	 */
	Vec3Df ambientLight() const;

	/**
	 * Calculates the light emitted from the surface towards the given vector.
	 * @param[in] outgoingVector The vector that the light is reflected towards.
	 * @return The light emitted from the surface towards the given vector.
	 */
	Vec3Df emittedLight(const Vec3Df &outgoingVector) const;

	/**
	* Calculates the light reflected from the incoming vector towards the outgoing vector.
	* @param[in] outgoingVector The vector that the light is reflected towards.
	* @param[in] incommingVector The vector from which the light is comming.
	* @param[in] lightColor The color of the light.
	* @return The light reflected from the incomming towards the outgoing vector.
	*/
	Vec3Df reflectedLight(const Vec3Df &outgoingVector, const Vec3Df &incommingVector, const Vec3Df &lightColor) const;

	/**
	 * Calculates the specularly reflected light towards the given vector.
	 * This is not physically correct.
	 * @param[in] outgoingVector The vector that the light is reflected towards.
	 * @return The light specularly reflected towards the outgoing vector.
	 */
	Vec3Df specularLight(const Vec3Df &outgoingVector) const;

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
	float u, v;

	/**
	* The geometry which this surface belongs to.
	*/
	const IGeometry *geometry;
};

#endif