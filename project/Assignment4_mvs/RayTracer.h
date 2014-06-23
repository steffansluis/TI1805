#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "IRayTracer.h"
#include "RayIntersection.h"

/**
 * Implements a whitted-style ray tracer.
 */
class RayTracer : public IRayTracer {
public:
	/**
	* Performs a ray tracing iteration.
	*
	* Traces the given ray through the scene and returns the light reflected backwards the ray.
	* Stops recursion when iteration reaches the max iterations limit.
	*
	* @param[in] origin	The origin of the ray.
	* @param[in] dir		The direction of the ray.
	* @param[in] iteration	The current iteration.
	* @param[out] distance	The distance to the closest surface hit by the ray.
	* @return The light towards the given ray.
	*/
	Vec3Df performRayTracingIteration(
		const Vec3Df &origin,
		const Vec3Df &dir,
		int iteration,
		float &distance) const;

private:
	/**
	 * Calculates the light reflected towards the ray from the point of intersection.
	 *
	 * @param[in] intersection	The intersection point to shade.
	 * @param[in] iteration		The current iteration.
	 * @return The light reflected towards the ray from the point of intersection.
	 */
	Vec3Df performShading(const RayIntersection &intersection, int iteration) const;
};

#endif