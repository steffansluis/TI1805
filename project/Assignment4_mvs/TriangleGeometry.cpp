#include "TriangleGeometry.h"

TriangleGeometry::TriangleGeometry(const Vec3Df &v0, const Vec3Df &v1, const Vec3Df &v2)
: vertex0(v0), vertex1(v1), vertex2(v2) {
}

Vec3Df TriangleGeometry::getVertex0() const {
	return this->vertex0;
}

Vec3Df TriangleGeometry::getVertex1() const {
	return this->vertex1;
}

Vec3Df TriangleGeometry::getVertex2() const {
	return this->vertex2;
}