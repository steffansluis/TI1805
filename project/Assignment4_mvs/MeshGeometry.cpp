#include <algorithm>
#include <cassert>
#include <math.h>

#include "IAccelerationStructure.h"
#include "mesh.h"
#include "MeshGeometry.h"
#include "MeshTriangleGeometry.h"
#include "NoAccelerationStructure.h"
#include "Octree.h"
#include "Random.h"
#include "SurfacePoint.h"

MeshGeometry::MeshGeometry(const Mesh *mesh) : 
accelerator(nullptr),
boundingBox(BoundingBox()),
maxTriangleArea(0),
mesh(mesh),
totalArea(0),
triangles(MeshGeometry::generateTriangles(mesh)){
	assert(mesh);

	this->setAccelerationStructure(std::make_shared<Octree>());
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
	float totalArea = 0.0f;
	float maxTriangleArea = 0.0f;

	// Preprocess all triangles and calculate the total surface area and
	// the surface area of the biggest triangle
	for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = this->triangles->begin(); it != this->triangles->end(); ++it) {
		(*it)->preprocess();

		float area = (*it)->getArea();
		totalArea += area;
		maxTriangleArea = std::max<float>(area, maxTriangleArea);
	}

	// Set the total surface area and maximum surface area of a triangle
	this->totalArea = totalArea;
	this->maxTriangleArea = maxTriangleArea;

	// Compute the bounding box
	this->boundingBox = MeshGeometry::createBoundingBox(this->mesh);

	// Preprocess the acceleration structure
	this->accelerator->preprocess();
}

float MeshGeometry::getArea() const {
	return this->totalArea;
}

bool MeshGeometry::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	// If the ray does not intersect the bounding box, return null
	if (!this->boundingBox.intersects(origin, dir))
		return false;

	// Let the acceleration structure handle the intersection in our set of triangles
	return this->accelerator->calculateClosestIntersection(origin, dir, intersection);
}

bool MeshGeometry::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	float distance;

	// If the ray does not intersect the bounding box within the given maximum distance, return null
	if (!this->boundingBox.intersects(origin, dir, distance) || distance > maxDistance)
		return false;

	// Let the acceleration structure handle the intersection in our set of triangles
	return this->accelerator->calculateAnyIntersection(origin, dir, maxDistance, intersection);
}

void MeshGeometry::getSurfacePoint(const RayIntersection &intersection, SurfacePoint &surface) const {
	// We are intersecting against the triangles within the mesh, so the geometry pointer of RayIntersection
	// will not point to the mesh geometry but instead to the triangle. There is no real need to implement this.
	assert(false);
}

void MeshGeometry::getRandomSurfacePoint(SurfacePoint &surface) const {
	// Get the number of triangles
	int numTriangles = this->mesh->triangles.size();

	// Get the maxium surface area of a triangle and
	// gerate a random surface area in the range [0, maxArea]
	float maxArea = this->maxTriangleArea;
	float randomArea = Random::randUnit() * totalArea;

	std::shared_ptr<IGeometry> triangle;

	// Use rejection sampling on the triangle's area
	// to pick a uniformly distributed random triangle
	do{
		// Get a random triangle
		int index = Random::rand() % numTriangles;
		triangle = this->triangles->at(index);
	} while (triangle->getArea() < randomArea);

	// Return a uniformly distributed random within the triangle
	return triangle->getRandomSurfacePoint(surface);
}

BoundingBox MeshGeometry::getBoundingBox() const {
	return this->boundingBox;
}

BoundingBox MeshGeometry::createBoundingBox(const Mesh *mesh) {
	// Construct an empty bounding box
	BoundingBox result = BoundingBox();

	// Insert all vertices
	for (unsigned int i = 0; i < mesh->vertices.size(); i++) {
		result.includePoint(mesh->vertices[i].p);
	}

	return result;
}

std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> MeshGeometry::generateTriangles(const Mesh *mesh) {
	auto triangles = std::make_shared<std::vector<std::shared_ptr<IGeometry>>>();

	// Create a MeshTriangleGeometry for each triangle
	for (std::vector<Triangle>::const_iterator it = mesh->triangles.begin(); it != mesh->triangles.end(); ++it) {
		triangles->push_back(std::make_shared<MeshTriangleGeometry>(mesh, &(*it)));
	}

	return triangles;
}