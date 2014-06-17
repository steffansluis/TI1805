#include <cassert>
#include <vector>

#include "ILight.h"
#include "RayTracer.h"
#include "RayIntersection.h"
#include "Scene.h"
#include "SurfacePoint.h"
#include "Vec3D.h"

// Returns the color of your pixel.
Vec3Df RayTracer::performRayTracing(const Vec3Df &origin, const Vec3Df &dir) {

	// print the iteration
	if (this->iterationCounter % 100 == 0)
	{
		std::cout << "iteration: " << this->iterationCounter << "\n";
	}

	this->iterationCounter++;
	

	// call the first iteration of the ray-tracing
	return this->performRayTracingIteration(origin, dir, 0);
}



void RayTracer::resetCounter()
{
	this->iterationCounter = 0;
}






// @Author: Bas Boellaard
// This performs ray-tracing and has an iteration for limited amount of iterations. 
// If you need your method to do a color-lookup for another ray, please call this method with an 
// incremented iteration-count.
Vec3Df RayTracer::performRayTracingIteration(const Vec3Df &origin, const Vec3Df &dir, const int iteration) const {
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

	// Execute all the different graphics techniques.
	return this->performShading(intersection, iteration);
}

// @Author: Martijn van Dorp
// Performs basic whitted-style shading.
Vec3Df RayTracer::performShading(const RayIntersection &intersection, const int iteration) const {
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
	lighting += surface.specularLight(viewVector, scene);

	// Iterate through all lights and sum the reflected light
	for (std::vector<std::shared_ptr<ILight>>::const_iterator it = lights->begin(); it != lights->end(); ++it) {
		// Get the light color and the light vector
		Vec3Df lightPoint;
		Vec3Df lightColor;
		Vec3Df lightVector;
		
		// Sample the light for a position and color
		(*it)->sampleLight(surface.point, lightPoint, lightColor);

		// Compute the vector from the light towards the intersection
		lightVector = (surface.point - lightPoint);
		float lightDistance = lightVector.normalize();

		const float smallNumber = 1e-6f;

		// If the segment between the light and intersection intersects any geometry
		// then the intersection is in shadow, continue to the next light.
		if (scene->calculateAnyIntersection(lightPoint + lightVector * smallNumber, lightVector, lightDistance - 2.0f * smallNumber, shadowIntersection))
			continue;

		// Evaluate the BRDF, essentially
		lighting += surface.reflectedLight(lightVector, viewVector, lightColor);
	}
	
	// Return the accumulated lighting
	return lighting;
}