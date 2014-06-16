#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Vec3D.h"

class Image;
class IAccelerationStructure;
class IGeometry;
class ICamera;
class ILight;
class IRayTracer;
class RayIntersection;

/**
 * Represents a scene of 3D objects which can be rendered to an image.
 */
class Scene {
public:
	Scene();
	~Scene();

	/*
	 * Finds the closest object hit by the given ray and returns a pointer to the RayIntersection representing
	 * the intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @return A pointer to a RayIntersection representing the intersection point of the ray.
	 */
	RayIntersection *calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const;

	/*
	 * Adds a geometrical object to the scene.
	 * @param[in] geometry Pointer to an IGeometry.
	 */
	void addGeometry(IGeometry *geometry);


	/*
	* Adds a light to the scene.
	* @param[in] geometry Pointer to an ILight.
	*/
	void addLight(ILight *light);

	/**
	 * Gets the vector containing all geometry in the scene.
	 * @return Pointer to a vector containing all the geometry in the scene.
	 */
	const std::vector<IGeometry*> *getGeometry() const;

	/**
	 * Gets the vector containing all lights in the scene.
	 * @return Pointer to a vector containing all the lights in the scene.
	 */
	const std::vector<ILight*> *getLights() const;

	/**
	 * Gets the acceleration structure that is used to find speed up
	 * the intersection calculations.
	 * @return Pointer to an acceleration structure.
	 */
	IAccelerationStructure *getAccelerationStructure() const;

	/**
	 * Sets the acceleration structure that is used to find speed up
	 * the intersection calculations.
	 * @param[in] accelerator Pointer to an acceleration structure.
	 * @return Pointer to the previous acceleration structure.
	 */
	IAccelerationStructure *setAccelerationStructure(IAccelerationStructure *accelerator);

	/**
	 * Gets the ray tracer that is used to trace rays through the scene.
	 * @return Pointer to a ray tracer.
	 */
	IRayTracer *getRayTracer() const;

	/**
	 * Sets the ray tracer that is used to trace rays through the scene.
	 * @param[in] rayTracer Pointer to ray tracer.
	 * @return Pointer to the previous ray tracer.
	 */
	IRayTracer *setRayTracer(IRayTracer *rayTracer);

	/**
	 * Renders the scene as seen from the given camera.
	 * @param[in] camera Pointer to the camera that observes the scene.
	 * @param width The width of the render.
	 * @param height The height of the render.
	 * @return Pointer to an image containing the rendered scene.
	 */
	Image *render(ICamera *camera, int width, int height);

private:
	/**
	* Perform any preprocessing necessary before rendering the scene.
	*/
	void preprocess();

	IAccelerationStructure *accelerator;
	IRayTracer *rayTracer;
	std::vector<IGeometry*> geometry;
	std::vector<ILight*> lights;
};

#endif