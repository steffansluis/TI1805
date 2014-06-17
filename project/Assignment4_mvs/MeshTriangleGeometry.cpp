#include <cassert>

#include "MeshGeometry.h"
#include "MeshTriangleGeometry.h"
#include "mesh.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

MeshTriangleGeometry::MeshTriangleGeometry(const Mesh *mesh, const Triangle *triangle)
: mesh(mesh), triangle(triangle) {
	assert(mesh);
	assert(triangle);
}

Vec3Df MeshTriangleGeometry::getVertex0() const {
	return this->mesh->vertices[this->triangle->v[0]].p;
}

Vec3Df MeshTriangleGeometry::getVertex1() const {
	return this->mesh->vertices[this->triangle->v[1]].p;
}

Vec3Df MeshTriangleGeometry::getVertex2() const {
	return this->mesh->vertices[this->triangle->v[2]].p;
}

void MeshTriangleGeometry::getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const {
	BaseTriangleGeometry::getSurfacePoint(intersection, surface);

	// Get the normal by interpolating the vertex normals
	// surface->normal = this->getSurfaceNormal(surface->u, surface->v);

	// Find the texture coordinates
	// this->getTextureCoordinates(surface->u, surface->v, surface->u, surface->v);
}

void MeshTriangleGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	BaseTriangleGeometry::getRandomSurfacePoint(surface);

	// Get the normal by interpolating the vertex normals
	// surface->normal = this->getSurfaceNormal(surface->u, surface->v);

	// Find the texture coordinates
	// this->getTextureCoordinates(surface->u, surface->v, surface->u, surface->v);
}

Vec3Df MeshTriangleGeometry::getSurfaceNormal(const Vec2Df &uv) const {
	// Get the vertex normals
	Vec3Df normal0 = this->mesh->vertices[this->triangle->v[0]].n;
	Vec3Df normal1 = this->mesh->vertices[this->triangle->v[1]].n;
	Vec3Df normal2 = this->mesh->vertices[this->triangle->v[2]].n;

	// Interpolate between the vertices
	Vec3Df normal = normal0 + uv[0] * normal1 + uv[1] * normal2;

	normal.normalize();

	return normal;
}

void MeshTriangleGeometry::getTextureCoordinates(const Vec2Df &uv, Vec2Df &tUV) const {
	// Check if the mesh has texture coordinates
	if (mesh->texcoords.size() > 0) {
		// Get the vertex texture coordinates
		Vec3Df uv0 = this->mesh->texcoords[this->triangle->v[0]];
		Vec3Df uv1 = this->mesh->texcoords[this->triangle->v[1]];
		Vec3Df uv2 = this->mesh->texcoords[this->triangle->v[2]];

		// Interpolate between the vertices
		Vec3Df uv3 = uv0 + uv[0] * uv1 + uv[1] * uv2;

		tUV[0] = uv3[0];
		tUV[1] = uv3[1];
	}
	// Otherwise return the barycentric coordinates
	else {
		tUV[0] = uv[0];
		tUV[1] = uv[1];
	}
}