#include <assert.h>
#include <math.h>

#include "IAccelerationStructure.h"
#include "mesh.h"
#include "MeshGeometry.h"
#include "MeshTriangleGeometry.h"
#include "NoAccelerationStructure.h"
#include "SurfacePoint.h"

MeshGeometry::MeshGeometry(const Mesh *mesh)
: mesh(mesh), triangles(std::vector<IGeometry *>()) {
	assert(mesh);

	this->setAccelerationStructure(new NoAccelerationStructure());

	// Create a MeshTriangleGeometry for each triangle
	for (std::vector<Triangle>::const_iterator it = this->mesh->triangles.begin(); it != this->mesh->triangles.end(); ++it) {
		this->triangles.push_back(new MeshTriangleGeometry(mesh, &(*it)));
	}
}

MeshGeometry::~MeshGeometry() {
	// Delete all MeshTriangleGeometry objects
	for (std::vector<IGeometry *>::iterator it = this->triangles.begin(); it != this->triangles.end(); ++it) {
		delete (*it);
	}
}

IAccelerationStructure *MeshGeometry::getAccelerationStructure() const {
	// Return the pointer to the acceleration structure
	return this->accelerator;
}

IAccelerationStructure *MeshGeometry::setAccelerationStructure(IAccelerationStructure *accelerator) {
	assert(accelerator);

	// Store the old acceleration structure pointer
	IAccelerationStructure *oldAccelerator = this->accelerator;

	// Set the acceleration structure and set its geometry vector to the scene's geometry
	this->accelerator = accelerator;
	this->accelerator->setGeometry(&this->triangles);

	// Return the pointer to the old acceleration structure
	return oldAccelerator;
}

void MeshGeometry::preprocess() {
	// Preprocess all triangles
	for (std::vector<IGeometry *>::iterator it = this->triangles.begin(); it != this->triangles.end(); ++it) {
		(*it)->preprocess();
	}

	//Preprocess the acceleration structure
	this->accelerator->preprocess();
}

RayIntersection *MeshGeometry::calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const {
	// Let the acceleration structure handle the intersection in our set of triangles
	return this->accelerator->calculateIntersection(origin, dir);
}

const SurfacePoint *MeshGeometry::getSurfacePoint(const RayIntersection *intersection) const {
	// We are intersecting against the triangles within the mesh, so the geometry pointer of RayIntersection
	// will not point to the mesh geometry but instead to the triangle. There is no real need to implement this.
	assert(false);

	return NULL;
}

const SurfacePoint *MeshGeometry::getRandomSurfacePoint() const {
	// BUG: Sampling is not uniform, needs to be weighted by triangle's surface area / total surface area.

	// Pick a random triangle
	int size = this->mesh->triangles.size();
	int index = rand() % size;

	// Return a random point on this triangle
	return this->triangles[index]->getRandomSurfacePoint();
}