#ifndef ICAMERA_H
#define ICAMERA_H

#include "Vec3D.h"

/**
 * Represents a camera.
 */
class ICamera {
public:
	/**
	 * Initializes a camera.
	 */
	ICamera();

	/**
	 * Initializes a camera with the given position and target.
	 * @param[in] position Position of the camera.
	 * @param[in] lookAt Point which the camera is looking at. 
	 */
	ICamera(const Vec3Df &position, const Vec3Df &lookAt);
	
	virtual ~ICamera();

	float getAspectRatio() const;
	Vec3Df getLookAt() const;
	Vec3Df getPosition() const;
	Vec3Df getUp() const;

	void setLookAt(const Vec3Df &lookAt);
	void setPosition(const Vec3Df &position);
	void setUp(const Vec3Df &up);

	/**
	 * Gets a ray through the given pixel.
	 * @param x The x-coordinate of the pixel.
	 * @param y The y-coordinate of the pixel.
	 * @param[out] origin The origin of the ray.
	 * @param[out] dir The direction of the ray.
	 */
	void getRay(int x, int y, Vec3Df &origin, Vec3Df &dir) const;

	/**
	* Gets a ray through the given pixel.
	* @param x The x-coordinate of the pixel.
	* @param y The y-coordinate of the pixel.
	* @param subPixelX X-offset within the pixel itself, range [0, 1].
	* @param subPixelY Y-offset within the pixel itself, range [0, 1].
	* @param[out] origin The origin of the ray.
	* @param[out] dir The direction of the ray.
	* @remarks The sub-pixel offset is for multisampling.
	*/
	void getRay(int x, int y, float subPixelX, float subPixelY, Vec3Df &origin, Vec3Df &dir) const;

	/**
	* Perform any necessary preprocessing.
	*/
	virtual void preprocess(int width, int height);

protected:
	/**
	* Gets a ray through the coordinates of the image plane.
	* @param y The u-coordinate of the image plane, range [-0.5, 0.5].
	* @param v The v-coordinate of the image plane, range [-0.5, 0.5].
	* @param[out] origin The origin of the ray.
	* @param[out] dir The direction of the ray.
	*/
	virtual void getRay(float u, float v, Vec3Df &origin, Vec3Df &dir) const = 0;

private:
	float aspectRatio;
	float invWidth;
	float invHeight;
	Vec3Df lookAt;
	Vec3Df position;
	Vec3Df up;
};

#endif