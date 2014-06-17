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
	* Traces the given ray through the scene and returns the light reflected tkwards the ray.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @return The light towards the given ray.
	*/
	Vec3Df performRayTracing(const Vec3Df &origin, const Vec3Df &dir);


	void resetCounter();


private:

	// A counter for the iteration of the ray-tracing algorithm. 
	int iterationCounter = 0;


	/**
	* Traces the given ray through the scene and returns the light reflected tkwards the ray.
	* Stops recursion when iteration reaches the max iterations limit.
	* @param[in] origin The origin of the ray.
	* @param[in] dir The direction of the ray.
	* @param iteration The current iteration.
	* @return The light towards the given ray.
	*/
	Vec3Df performRayTracingIteration(const Vec3Df &origin, const Vec3Df &dir, const int iteration) const;

	/**
	 * Calculates the light reflected towards the ray from the point of intersection.
	 * @param iteration The current iteration.
	 * @return The light reflected towards the ray from the point of intersection.
	 */
	Vec3Df performShading(std::shared_ptr<const RayIntersection> intersection, const int iteration) const;

	// Indicate the maximum amount of iterations that are permitted.
	// This prevent the algorithm from looping back and forth between objects infinitely.
	const int maxIterations = 16;
};

#endif