#include <assert.h>
#include <math.h>

#include "IAccelerationStructure.h"
#include "DiffuseMaterial.h"
#include "mesh.h"
#include "MeshGeometry.h"
#include "MeshTriangleGeometry.h"
#include "NoAccelerationStructure.h"
#include "SurfacePoint.h"

MeshGeometry::MeshGeometry(const Mesh *mesh)
: mesh(mesh), triangles(MeshGeometry::generateTriangles(mesh)) {
	assert(mesh);

	this->setAccelerationStructure(std::make_shared<NoAccelerationStructure>());
}

MeshGeometry::~MeshGeometry() {
}

std::shared_ptr<IAccelerationStructure> MeshGeometry::getAccelerationStructure() const {
	// Return the pointer to the acceleration structure
	return this->accelerator;
}

void MeshGeometry::setAccelerationStructure(std::shared_ptr<IAccelerationStructure> accelerator) {
	assert(accelerator);

	// Set the acceleration structure and set its geometry vector to the scene's geometry
	this->accelerator = accelerator;
	this->accelerator->setGeometry(this->triangles);
}

void MeshGeometry::preprocess() {
	// Preprocess all triangles
	for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = this->triangles->begin(); it != this->triangles->end(); ++it) {
		(*it)->preprocess();
	}

	//Preprocess the acceleration structure
	this->accelerator->preprocess();
}

std::shared_ptr<const RayIntersection> MeshGeometry::calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const {
	// Let the acceleration structure handle the intersection in our set of triangles
	return this->accelerator->calculateIntersection(origin, dir);
}

std::shared_ptr<const SurfacePoint> MeshGeometry::getSurfacePoint(std::shared_ptr<const RayIntersection> intersection) const {
	// We are intersecting against the triangles within the mesh, so the geometry pointer of RayIntersection
	// will not point to the mesh geometry but instead to the triangle. There is no real need to implement this.
	assert(false);

	return nullptr;
}

std::shared_ptr<const SurfacePoint> MeshGeometry::getRandomSurfacePoint() const {
	// BUG: Sampling is not uniform, needs to be weighted by triangle's surface area / total surface area.

	// Pick a random triangle
	int size = this->mesh->triangles.size();
	int index = rand() % size;

	// Return a random point on this triangle
	return this->triangles->at(index)->getRandomSurfacePoint();
}

std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> MeshGeometry::generateTriangles(const Mesh *mesh) {
	auto triangles = std::make_shared<std::vector<std::shared_ptr<IGeometry>>>();

	// Create a MeshTriangleGeometry for each triangle
	for (std::vector<Triangle>::const_iterator it = mesh->triangles.begin(); it != mesh->triangles.end(); ++it) {
		triangles->push_back(std::make_shared<MeshTriangleGeometry>(mesh, &(*it)));
	}

	return triangles;
}