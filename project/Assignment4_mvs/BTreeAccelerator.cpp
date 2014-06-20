#include "BoundingBox.h"
#include "BTreeAccelerator.h"
#include "IGeometry.h"
#include "NoAccelerationStructure.h"
#include "RayIntersection.h"

#include <algorithm>
#include <limits>

// A custom deleter for shared_ptr which does not delete anything.
void null_deleter(std::vector<std::shared_ptr<IGeometry>> *) { }

BTreeAccelerator::BTreeAccelerator()
{
	xTree = new BTree(BTree::Coordinate::X);
	yTree = new BTree(BTree::Coordinate::Y);
	zTree = new BTree(BTree::Coordinate::Z);
}

void BTreeAccelerator::preprocess() 
{
	// create new trees;
	xTree = new BTree(BTree::Coordinate::X);
	yTree = new BTree(BTree::Coordinate::Y);
	zTree = new BTree(BTree::Coordinate::Z);

	// the geometry
	std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>> geometry = this->getGeometry();

	float infinity = std::numeric_limits<float>::infinity();

	// initialize the minimum and maximum values for the coordinates x, y, z
	this->xBounds[0] = infinity;	// minimum
	this->xBounds[1] = -infinity;	// maximum
	this->yBounds[0] = infinity;	// minimum
	this->yBounds[1] = -infinity;	// maximum
	this->zBounds[0] = infinity;	// minimum
	this->zBounds[1] = -infinity;	// maximum

	// Iterate through all geometry in the scene
	for (unsigned int i = 0; i < geometry->size(); i++)
	{
		// The BTreeAccelerator was designed for triangles.
		// Try parsing the geometry to a triangle.
		// If this fails, throw a custom exception
		try
		{
			std::shared_ptr<IGeometry> object = geometry->at(i);

			// add the triangle to the btrees
			this->xTree->AddNode(object);
			this->yTree->AddNode(object);
			this->zTree->AddNode(object);

			// get the bounds of the object
			BoundingBox bounds = object->getBoundingBox();

			//take the smallest x, y and z as minimum bounds
			this->xBounds[0] = std::min(this->xBounds[0], bounds.min[0]);
			this->yBounds[0] = std::min(this->yBounds[0], bounds.min[1]);
			this->zBounds[0] = std::min(this->zBounds[0], bounds.min[2]);

			// take the largest x, y and z as maximum bounds
			this->xBounds[1] = std::max(this->xBounds[1], bounds.max[0]);
			this->yBounds[1] = std::max(this->yBounds[1], bounds.max[1]);
			this->zBounds[1] = std::max(this->zBounds[1], bounds.max[2]);
		}
		catch (std::exception e)
		{
			throw BTreeAcceleratorException(e.what());
		}
	}
}

bool BTreeAccelerator::calculateClosestIntersection(const Vec3Df &origin, const Vec3Df &dir, RayIntersection &intersection) const {

	// take 'origin' as the zero-vector and calculate the destination
	Vec3Df oDestination = dir - origin;

	// calculate the coordinate scalar for all three coordinates
	float xScalar = this->calculateCoordinateScalar(origin, oDestination, BTree::Coordinate::X);
	float yScalar = this->calculateCoordinateScalar(origin, oDestination, BTree::Coordinate::Y);
	float zScalar = this->calculateCoordinateScalar(origin, oDestination, BTree::Coordinate::Z);

	// with this scalar, calculate the bounds of the coordinates
	float xLimit1 = dir[0];
	float yLimit1 = dir[1];
	float zLimit1 = dir[2];
	//
	float xLimit2 = dir[0] + xScalar * oDestination[0];
	float yLimit2 = dir[1] + yScalar * oDestination[1];
	float zLimit2 = dir[2] + zScalar * oDestination[2];

	// get the triangles that correspond to these limits
	std::vector<std::shared_ptr<IGeometry>> xTriangles = this->xTree->GetTriangles(xLimit1, xLimit2);
	std::vector<std::shared_ptr<IGeometry>> yTriangles = this->yTree->GetTriangles(xLimit1, xLimit2);
	std::vector<std::shared_ptr<IGeometry>> zTriangles = this->zTree->GetTriangles(xLimit1, xLimit2);

	// calculate the intersection of these three collections
	int intersec1Size = std::min(xTriangles.size(), yTriangles.size());
	std::vector<std::shared_ptr<IGeometry>> intersec1 = std::vector<std::shared_ptr<IGeometry>>(intersec1Size);
	std::set_intersection(xTriangles.begin(), xTriangles.end(), yTriangles.begin(), yTriangles.end(), intersec1.begin());
	//
	int myIntersectionSize = std::min(intersec1.size(), zTriangles.size());
	std::vector<std::shared_ptr<IGeometry>> myIntersection = std::vector<std::shared_ptr<IGeometry>>(myIntersectionSize);
	std::set_intersection(intersec1.begin(), intersec1.end(), zTriangles.begin(), zTriangles.end(), myIntersection.begin());

	// feed the triangles to the ray-intersection algorithm


	// Not the most elegant solution but it should work
	Vec3Df dest = dir * 10000.0f;

	// Use the retrieveTriangles to find a list of potential intersecting objects
	std::vector<std::shared_ptr<IGeometry>> candidateSet = this->retrieveTriangles(origin, dest);

	// Construct a NoAccelerationStructure to perform a simple intersection test
	NoAccelerationStructure simpleIntersectionAlgorithm;
	simpleIntersectionAlgorithm.setGeometry(std::shared_ptr<const std::vector<std::shared_ptr<IGeometry>>>(&myIntersection, null_deleter));
	simpleIntersectionAlgorithm.preprocess();

	// Use the simple intersection algorithm to find the closest intersections
	return simpleIntersectionAlgorithm.calculateClosestIntersection(origin, dir, intersection);
}

float BTreeAccelerator::calculateCoordinateScalar(const Vec3Df& origin, const Vec3Df & destination, BTree::Coordinate coordinate) const
{
	float coord2Value;
	float coord2Extreme;

	float coord3Value;
	float coord3Extreme;

	switch (coordinate)
	{
	case BTree::Coordinate::X:
		coord2Value = destination[1];
		coord2Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Y);

		coord3Value = destination[2];
		coord3Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Z);

		break;
	case BTree::Coordinate::Y:
		coord2Value = destination[0];
		coord2Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::X);

		coord3Value = destination[2];
		coord3Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Z);


		break;
	case BTree::Coordinate::Z:
		coord2Value = destination[0];
		coord2Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::X);

		coord3Value = destination[1];
		coord3Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Y);

		break;
	}

	// see which of the two coordinates has the larger scalar and return the value
	return std::max(coord2Extreme / coord2Value, coord3Extreme / coord3Value);
}

// @Author: Bas Boellaard
// This method gets the extreme for the specified coordinate coming from origin 'origin' with destination 'destination'. 
// If the arity of the maximum is the same as the arity of the vector for that coordinate, then that is the corresponding 
// extreme.
//
// The origin is subtracted since the extreme must be relative to the origin in order to test arity. 
// The destination is expected to already be relative to the origin.
float BTreeAccelerator::GetCoordinateExtreme(const Vec3Df& origin, const Vec3Df & destination, BTree::Coordinate coordinate) const
{
	switch (coordinate)
	{
	case BTree::Coordinate::X:
		if ((this->xBounds[1] - origin[0]) / destination[0] > 0)
		{
			// same arity, the maximum is the bound
			return this->xBounds[1];
		}
		else
		{
			// return the minimum instead.
			return this->xBounds[0];
		}

	case BTree::Coordinate::Y:
		if ((this->yBounds[1] - origin[1]) / destination[1] > 0)
		{
			// same arity, the maximum is the bound
			return this->yBounds[1];
		}
		else
		{
			// return the minimum instead.
			return this->yBounds[0];
		}

	case BTree::Coordinate::Z:
		if ((this->zBounds[1] - origin[2]) / destination[2] > 0)
		{
			// same arity, the maximum is the bound
			return this->zBounds[1];
		}
		else
		{
			// return the minimum instead.
			return this->zBounds[0];
		}

	default:
		// perhaps raise an excepion here
		return 0.0f;
	}
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
		myTriangles = xTree->GetTriangles(lowerLimit, upperLimit);
		break;
	case BTree::Coordinate::Y:
		myTriangles = yTree->GetTriangles(lowerLimit, upperLimit);
		break;
	case BTree::Coordinate::Z:
		myTriangles = zTree->GetTriangles(lowerLimit, upperLimit);
		break;

	default:
		myTriangles = std::vector<std::shared_ptr<IGeometry>>();
	}

	return myTriangles;
}


///////////////// BTreeAcceleratorException //////////////////////

BTreeAcceleratorException::BTreeAcceleratorException(const char* message)
{
	this->_message = message;
}

const char* BTreeAcceleratorException::what() const throw()
{
	return this->_message;
}