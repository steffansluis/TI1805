#ifndef TRIANGLEGEOMETRY_H
#define TRIANGLEGEOMETRY_H

#include "BaseTriangleGeometry.h"
#include "Vec3D.h"

/**
 * Represents a triangle.
 */
class TriangleGeometry : public BaseTriangleGeometry {
public:
	/**
	 * Initializes a TriangleGeometry with the given vertices.
	 * @param[in] v0 The first vertex.
	 * @param[in] v1 The second vertex.
	 * @param[in] v2 The third vertex.
	 */
	TriangleGeometry(const Vec3Df &v0, const Vec3Df &v1, const Vec3Df &v2);

	Vec3Df getVertex0() const;
	Vec3Df getVertex1() const;
	Vec3Df getVertex2() const;

private:
	Vec3Df vertex0;
	Vec3Df vertex1;
	Vec3Df vertex2;
};

#endif