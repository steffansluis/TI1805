#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "Vec3D.h"

class Image;
class IAccelerationStructure;
class ICamera;
class IGeometry;
class ILight;
class IRayTracer;
class RayIntersection;

/**
 * Represents a scene of 3D objects which can be rendered to an image.
 */
class Scene : std::enable_shared_from_this<const Scene> {
public:
	Scene();
	~Scene();

	/*
	 * Returns whether any object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the closest point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	bool calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const;

	/*
	 * Returns whether any object is hit by the given ray and sets the intersection parameter
	 * to the RayIntersection representing the point of intersection.
	 * @param[in] origin The origin of the ray.
	 * @param[in] dir The direction of the ray.
	 * @param maxDistance The maximum distance at which the intersection may occur.
	 * @param[out] intersection Reference to a RayIntersection representing the intersection point of the ray.
	 * @return True if the ray intersected an object; otherwise false.
	 */
	bool calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const;
	
	/*
	 * Adds a geometrical object to the scene.
	 * @param[in] geometry Pointer to an IGeometry.
	 */
	void addGeometry(std::shared_ptr<IGeometry> geometry);

	/*
	 * Adds a light to the scene.
	 * @param[in] geometry Pointer to an ILight.
	 */
	void addLight(std::shared_ptr<ILight> light);

	/**
	 * Gets the vector containing all geometry in the scene.
	 * @return Pointer to a vector containing all the geometry in the scene.
	 */
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> getGeometry() const;

	/**
	 * Gets the vector containing all lights in the scene.
	 * @return Pointer to a vector containing all the lights in the scene.
	 */
	std::shared_ptr<const std::vector<std::shared_ptr<ILight>>> getLights() const;

	/**
	 * Gets the acceleration structure that is used to find speed up
	 * the intersection calculations.
	 * @return Pointer to an acceleration structure.
	 */
	std::shared_ptr<const IAccelerationStructure> getAccelerationStructure() const;

	/**
	 * Gets whether or not path tracing is enabled.
	 * @return Whether or not path tracing is enabled.
	 */
	bool getPathTracingEnabled() const;

	/**
	 * Gets the number of light samples to be taken per square unit of surface area.
	 * @return The number of light samples to be taken per square unit of surface area.
	 */
	float getLightSampleDensity() const;

	/**
	 * Gets the ambient light color.
	 */
	Vec3Df getAmbientLight() const;

	/**
	* Gets the number of ambient occlusion samples to be taken.
	* @return The number of ambient occlusion samples to be taken.
	*/
	int getAmbientOcclusionSamples() const;

	/**
	* Gets the square root of the number of samples taken per pixel.
	* @return The square root of the number of samples taken per pixel.
	*/
	int getSamplesPerPixel() const;

	/**
	* Gets the maximum ray tracing recursion depth.
	* @return The maximum ray tracing recursion depth.
	*/
	int getMaxTraceDepth() const;

	/**
	 * Sets the acceleration structure that is used to find speed up
	 * the intersection calculations.
	 * @param[in] accelerator Pointer to an acceleration structure.
	 */
	void setAccelerationStructure(std::shared_ptr<IAccelerationStructure> accelerator);

	/**
	 * Gets the ray tracer that is used to trace rays through the scene.
	 * @return Pointer to a ray tracer.
	 */
	std::shared_ptr<const IRayTracer> getRayTracer() const;

	/**
	 * Sets the ray tracer that is used to trace rays through the scene.
	 * @param[in] rayTracer Pointer to ray tracer.
	 * @return Pointer to the previous ray tracer.
	 */
	void setRayTracer(std::shared_ptr<IRayTracer> rayTracer);

	/**
	* Sets the number of light samples to be taken per square unit of surface area.
	* @param density The number of light samples to be taken per square unit of surface area.
	*/
	void setLightSampleDensity(float density);

	/**
	 * Sets the ambient light color.
	 */
	void setAmbientLight(const Vec3Df &ambientLight);

	/**
	* Sets the number of ambient occlusion samples to be taken.
	* @param numSamples The number of ambient occlusion samples to be taken.
	*/
	void setAmbientOcclusionSamples(int numSamples);

	/**
	* Sets the square root of the number of samples taken per pixel.
	* @param numSamples The square root of the number of samples taken per pixel.
	*/
	void setSamplesPerPixel(int numSamples);

	/**
	* Sets the maximum ray tracing recursion depth.
	* @param maxDepth The maximum ray tracing recursion depth.
	*/
	void setMaxTraceDepth(int maxDepth);

	/**
	* Sets whether or not path tracing is enabled.
	* @param enabled Whether or not path tracing is enabled.
	*/
	void setPathTracingEnabled(bool enabled);

	/**
	 * Renders the scene as seen from the given camera.
	 * @param[in] camera Pointer to the camera that observes the scene.
	 * @param width The width of the render.
	 * @param height The height of the render.
	 * @return Pointer to an image containing the rendered scene.
	 */
	std::shared_ptr<Image> render(std::shared_ptr<ICamera>, int width, int height);

private:
	/**
	 * Perform any preprocessing necessary before rendering the scene.
	 */
	void preprocess();

	Vec3Df renderPixel(std::shared_ptr<ICamera> camera, int x, int y);

	bool pathTracingEnabled;
	int ambientOcclusionSamples;
	int samplesPerPixel;
	int maxTraceDepth;
	float lightSampleDensity;
	Vec3Df ambientLight;
	std::shared_ptr<IAccelerationStructure> accelerator;
	std::shared_ptr<IRayTracer> rayTracer;
	std::shared_ptr<std::vector<std::shared_ptr<IGeometry>>> geometry;
	std::shared_ptr<std::vector<std::shared_ptr<ILight>>> lights;
};

#endif