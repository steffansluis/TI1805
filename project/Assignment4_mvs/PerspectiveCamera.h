#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "ICamera.h"

/**
 * Represents a perspective camera.
 */
class PerspectiveCamera : public ICamera {
public:
	/**
	 * Initializes a camera.
	 */
	PerspectiveCamera();

	/**
	 * Initializes a camera with the given position and target.
	 * @param[in] position Position of the camera.
	 * @param[in] lookAt Point which the camera is looking at.
	 */
	PerspectiveCamera(const Vec3Df &position, const Vec3Df &lookAt);

	float getFieldOfView() const;
	void setFieldOfView(float fov);

	/**
	 * Perform any necessary preprocessing.
	 */
	void preprocess(int width, int height);

protected:
	/**
	 * Gets a ray through the coordinates of the image plane.
	 * @param y The u-coordinate of the image plane, range [-0.5, 0.5].
	 * @param v The v-coordinate of the image plane, range [-0.5, 0.5].
	 * @param[out] origin The origin of the ray.
	 * @param[out] dir The direction of the ray.
	 */
	void getRay(float u, float v, Vec3Df &origin, Vec3Df &dir) const;

private:
	float fieldOfView;
	Vec3Df right;
	Vec3Df up;
	Vec3Df forward;
	Vec3Df imagePlaneOffset;
};

#endif