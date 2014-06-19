#ifndef IRAYTRACER_H
#define IRAYTRACER_H

#include <memory>

#include "Vec3D.h"

class Scene;

/**
 * Represents a ray tracer.
 */
class IRayTracer {
public:
	virtual ~IRayTracer(){};

	/**
	 * Gets a pointer to the scene.
	 *
	 * @return A pointer to the scene.
	 */
	const Scene *getScene() const;

	/**
	 * Sets the pointer to the scene.
	 *
	 * @param[in] scene Pointer to a scene.
	 * @return A pointer to the old scene.
	 */
	void setScene(const Scene *scene);

	/**
	 * Traces the given ray through the scene and returns the light reflected backwards the ray.
	 *
	 * @param[in] origin	The origin of the ray.
	 * @param[in] dir		The direction of the ray.
	 * @return The light towards the given ray.
	 */
	virtual Vec3Df performRayTracing(const Vec3Df &origin, const Vec3Df &dir) = 0;

private:
	const Scene *scene;
};

#endif