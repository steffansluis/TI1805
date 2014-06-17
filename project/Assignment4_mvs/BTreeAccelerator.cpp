#include "BTreeAccelerator.h"
#include "IGeometry.h"
#include "NoAccelerationStructure.h"
#include "RayIntersection.h"

void null_deleter(std::vector<std::shared_ptr<IGeometry>> *) { }

BTreeAccelerator::BTreeAccelerator() 
: xTree(BTree::Coordinate::X), yTree(BTree::Coordinate::Y), zTree(BTree::Coordinate::Z) {
}

void BTreeAccelerator::preprocess() {
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

	// Iterate through all geometry in the scene
	for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = geometry->begin(); it != geometry->end(); ++it) {
		xTree.AddNode(*it);
		yTree.AddNode(*it);
		zTree.AddNode(*it);
	}
}

bool BTreeAccelerator::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {
	// Not the most elegant solution but it should work
	Vec3Df dest = dir * 10000.0f;

	// Use the retrieveTriangles to find a list of potential intersecting objects
	std::vector<std::shared_ptr<IGeometry>> candidateSet = this->retrieveTriangles(origin, dest);

	// Constructor a NoAccelerationStructure to perform a simple intersection test
	NoAccelerationStructure simpleIntersectionAlgorithm;
	simpleIntersectionAlgorithm.setGeometry(std::shared_ptr<std::vector<std::shared_ptr<IGeometry>>>(&candidateSet, null_deleter));
	simpleIntersectionAlgorithm.preprocess();

	// Use the simple intersection algorithm to find the closest intersections
	return simpleIntersectionAlgorithm.calculateClosestIntersection(origin, dir, intersection);
}

bool BTreeAccelerator::calculateAnyIntersection(const Vec3Df &origin, const Vec3Df &dir, float maxDistance, RayIntersection &intersection) const {
	Vec3Df dest = dir * maxDistance;

	// Use the retrieveTriangles to find a list of potential intersecting objects
	std::vector<std::shared_ptr<IGeometry>> candidateSet = this->retrieveTriangles(origin, dest);

	// Constructor a NoAccelerationStructure to perform a simple intersection test
	NoAccelerationStructure simpleIntersectionAlgorithm;
	simpleIntersectionAlgorithm.setGeometry(std::shared_ptr<std::vector<std::shared_ptr<IGeometry>>>(&candidateSet, null_deleter));
	simpleIntersectionAlgorithm.preprocess();

	// Use the simple intersection algorithm to find the closest intersections
	return simpleIntersectionAlgorithm.calculateAnyIntersection(origin, dir, maxDistance, intersection);
}

std::vector<std::shared_ptr<IGeometry>> BTreeAccelerator::retrieveTriangles(const Vec3Df & origin, const Vec3Df & dest) const {
	std::vector<std::shared_ptr<IGeometry>> myTriangles;

	bool USE_DEBUG_TRIANGLES = false;

	if (USE_DEBUG_TRIANGLES)
	{
		std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

		// Iterate through all geometry in the scene
		for (std::vector<std::shared_ptr<IGeometry>>::const_iterator it = geometry->begin(); it != geometry->end(); ++it) {
			myTriangles.push_back(*it);
		}

		return myTriangles;
	}

	// Figure out which coordinate {X, Y, Z} to use
	// THIS IS A DUMMY VALUE
	BTree::Coordinate coordinate = BTree::Coordinate::X;

	// figure out the bounds for the triangles.
	// THESE ARE DUMMY VALUES
	float lowerLimit = -1.0;
	float upperLimit = 1.0;

	switch (coordinate)
	{
	case BTree::Coordinate::X:
		myTriangles = xTree.GetTriangles(lowerLimit, upperLimit);
		break;
	case BTree::Coordinate::Y:
		myTriangles = yTree.GetTriangles(lowerLimit, upperLimit);
		break;
	case BTree::Coordinate::Z:
		myTriangles = zTree.GetTriangles(lowerLimit, upperLimit);
		break;

	default:
		myTriangles = std::vector<std::shared_ptr<IGeometry>>();
	}

	return myTriangles;
}