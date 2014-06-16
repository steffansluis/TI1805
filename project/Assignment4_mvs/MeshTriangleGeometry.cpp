#include <assert.h>

#include "MeshGeometry.h"
#include "MeshTriangleGeometry.h"
#include "mesh.h"
#include "SurfacePoint.h"

MeshTriangleGeometry::MeshTriangleGeometry(const MeshGeometry *mesh, const Triangle *triangle)
: mesh(mesh), triangle(triangle) {
	assert(mesh);
	assert(triangle);

	// !!! TEMPORARY
	this->setMaterial(mesh->getMaterial());
}

Vec3Df MeshTriangleGeometry::getVertex0() const {
	return this->mesh->getMesh()->vertices[this->triangle->v[0]].p;
}

Vec3Df MeshTriangleGeometry::getVertex1() const {
	return this->mesh->getMesh()->vertices[this->triangle->v[1]].p;
}

Vec3Df MeshTriangleGeometry::getVertex2() const {
	return this->mesh->getMesh()->vertices[this->triangle->v[2]].p;
}

const SurfacePoint *MeshTriangleGeometry::getSurfacePoint(const RayIntersection *intersection) const {
	SurfacePoint *surface = const_cast<SurfacePoint*>(BaseTriangleGeometry::getSurfacePoint(intersection));

	// Get the normal by interpolating the vertex normals
	// surface->normal = this->getSurfaceNormal(surface->u, surface->v);

	// Find the texture coordinates
	// this->getTextureCoordinates(surface->u, surface->v, surface->u, surface->v);

	return surface;
}

const SurfacePoint *MeshTriangleGeometry::getRandomSurfacePoint() const {
	SurfacePoint *surface = const_cast<SurfacePoint*>(BaseTriangleGeometry::getRandomSurfacePoint());

	// Get the normal by interpolating the vertex normals
	// surface->normal = this->getSurfaceNormal(surface->u, surface->v);

	// Find the texture coordinates
	// this->getTextureCoordinates(surface->u, surface->v, surface->u, surface->v);

	return surface;
}

Vec3Df MeshTriangleGeometry::getSurfaceNormal(float u, float v) const {
	// Get the mesh
	const Mesh *mesh = this->mesh->getMesh();

	// Get the vertex normals
	Vec3Df normal0 = mesh->vertices[this->triangle->v[0]].n;
	Vec3Df normal1 = mesh->vertices[this->triangle->v[1]].n;
	Vec3Df normal2 = mesh->vertices[this->triangle->v[2]].n;

	// Interpolate between the vertices
	Vec3Df normal = normal0 + u * normal1 + v * normal2;

	normal.normalize();

	return normal;
}

void MeshTriangleGeometry::getTextureCoordinates(float u, float v, float &tu, float &tv) const {
	// Get the mesh
	const Mesh *mesh = this->mesh->getMesh();

	// Check if the mesh has texture coordinates
	if (mesh->texcoords.size() > 0) {
		// Get the vertex texture coordinates
		Vec3Df uv0 = mesh->texcoords[this->triangle->v[0]];
		Vec3Df uv1 = mesh->texcoords[this->triangle->v[1]];
		Vec3Df uv2 = mesh->texcoords[this->triangle->v[2]];

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