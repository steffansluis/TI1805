#include <algorithm>
#include <cassert>
#include <vector>

#include "Constants.h"
#include "ILight.h"
#include "RayTracer.h"
#include "RayIntersection.h"
#include "Scene.h"
#include "SurfacePoint.h"
#include "Vec3D.h"

// @Author: Bas Boellaard
// This performs ray-tracing and has an iteration for limited amount of iterations. 
// If you need your method to do a color-lookup for another ray, please call this method with an 
// incremented iteration-count.
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
Vec3Df RayTracer::performRayTracingIteration(
	const Vec3Df &origin,
	const Vec3Df &dir,
	int iteration,
	float &distance) const
{
	assert(this->getScene());

	// If the maximum amount of iterations has been reached, return the zero-vector (black).
	if (iteration >= RayTracer::maxIterations)
	{
		return Vec3Df(0, 0, 0);
	}

	// Calculate the intersection that will occur with these parameters
	RayIntersection intersection;
	bool intersects = this->getScene()->calculateClosestIntersection(origin, dir, intersection);

	// If there was no intersection, return black
	if (!intersects)
	{
		return Vec3Df(0, 0, 0);
	}

	distance = intersection.distance;

	// Execute all the different graphics techniques.
	return this->performShading(intersection, iteration);
}

// @Author: Martijn van Dorp
// Performs basic whitted-style shading.
Vec3Df RayTracer::performShading(const RayIntersection &intersection, int iteration) const {
	// Needed for the shadow intersection test but can be ignored.
	RayIntersection shadowIntersection;

	// Get a pointer to the scene.
	const Scene *scene = this->getScene();

	// Get the surface point at the intersection point, this contains surface parameters useful for shading.
	SurfacePoint surface;
	intersection.getSurfacePoint(surface);

	// Get the vector contain the scene's lights.
	std::shared_ptr<const std::vector<std::shared_ptr<ILight>>> lights = scene->getLights();
	
	// The 'view' vector is the opposite of the ray direction
	Vec3Df viewVector = -intersection.direction;

	// Calculate ambient, emitted and specularly reflected light.
	Vec3Df lighting = Vec3Df();
	lighting += surface.ambientLight(this->getScene());
	lighting += surface.emittedLight(viewVector);
	lighting += surface.specularLight(viewVector, scene, iteration);

	// Iterate through all lights and sum the reflected light
	for (std::vector<std::shared_ptr<ILight>>::const_iterator it = lights->begin(); it != lights->end(); ++it) {
		// Get the light color and the light vector
		std::shared_ptr<ILight> light = (*it);
		Vec3Df lightPoint;
		Vec3Df lightColor;
		Vec3Df lightVector;
		Vec3Df lightContribution = Vec3Df();

		// Calculate the number of light sampels to be taken based of the light's area
		int lightSamples = (int)std::max(1.0f, light->getArea() * scene->getLightSampleDensity());

		// If the light is associated with the geometry we are trying to 
		if (light->getGeometry() == intersection.geometry)
			continue;

		for (int i = 0; i < lightSamples; i++) {
			// Sample the light for a position and color
			light->sampleLight(surface.point, lightPoint, lightColor);

			// Compute the vector from the intersection towards the light
			lightVector = (lightPoint - surface.point);
			float lightDistance = lightVector.normalize();

			// The length of the shadow ray segment, this is sligthly smaller
			// than the actual distance between the object and light to
			// prevent intersecting the object and lightsource themselves.
			float segmentLength = lightDistance - 2.0f * Constants::Epsilon;

			// If the light and object are extremely close then the segment length may become negative,
			// only check shadows if there is enough room between the object and light.
			if (segmentLength > 0.0f) {
				// If the segment between the light and intersection intersects any geometry
				// then the intersection is in shadow, continue to the next light.
				if (scene->calculateAnyIntersection(surface.point + lightVector * Constants::Epsilon, lightVector, segmentLength, shadowIntersection))
					continue;
			}

			// Evaluate the BRDF, essentially
			lightContribution += surface.reflectedLight(lightVector, viewVector, lightColor);
		}

		lighting += lightContribution / (float)lightSamples;
	}
	
	// Return the accumulated lighting
	return lighting;
}