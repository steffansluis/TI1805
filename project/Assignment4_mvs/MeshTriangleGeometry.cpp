#include <assert.h>

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

std::shared_ptr<const SurfacePoint> MeshTriangleGeometry::getSurfacePoint(std::shared_ptr<const RayIntersection> intersection) const {
	std::shared_ptr<SurfacePoint> surface = std::const_pointer_cast<SurfacePoint>(BaseTriangleGeometry::getSurfacePoint(intersection));

	// Get the normal by interpolating the vertex normals
	// surface->normal = this->getSurfaceNormal(surface->u, surface->v);

	// Find the texture coordinates
	// this->getTextureCoordinates(surface->u, surface->v, surface->u, surface->v);

	return surface;
}

std::shared_ptr<const SurfacePoint> MeshTriangleGeometry::getRandomSurfacePoint() const {
	std::shared_ptr<SurfacePoint> surface = std::const_pointer_cast<SurfacePoint>(BaseTriangleGeometry::getRandomSurfacePoint());

	// Get the normal by interpolating the vertex normals
	// surface->normal = this->getSurfaceNormal(surface->u, surface->v);

	// Find the texture coordinates
	// this->getTextureCoordinates(surface->u, surface->v, surface->u, surface->v);

	return surface;
}

Vec3Df MeshTriangleGeometry::getSurfaceNormal(float u, float v) const {
	// Get the vertex normals
	Vec3Df normal0 = this->mesh->vertices[this->triangle->v[0]].n;
	Vec3Df normal1 = this->mesh->vertices[this->triangle->v[1]].n;
	Vec3Df normal2 = this->mesh->vertices[this->triangle->v[2]].n;

	// Interpolate between the vertices
	Vec3Df normal = normal0 + u * normal1 + v * normal2;

	normal.normalize();

	return normal;
}

void MeshTriangleGeometry::getTextureCoordinates(float u, float v, float &tu, float &tv) const {
	// Check if the mesh has texture coordinates
	if (mesh->texcoords.size() > 0) {
		// Get the vertex texture coordinates
		Vec3Df uv0 = this->mesh->texcoords[this->triangle->v[0]];
		Vec3Df uv1 = this->mesh->texcoords[this->triangle->v[1]];
		Vec3Df uv2 = this->mesh->texcoords[this->triangle->v[2]];

		// Interpolate between the vertices
		Vec3Df uv = uv0 + u * uv1 + v * uv2;

		tu = uv[0];
		tv = uv[1];
	}
	// Otherwise return the barycentric coordinates
	else {
		tu = u;
		tv = v;
	}
}