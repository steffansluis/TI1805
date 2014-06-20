#include "BTreeAccelerator.h"
#include "IGeometry.h"
#include "NoAccelerationStructure.h"
#include "RayIntersection.h"

#include "TriangleGeometry.h"

#include <algorithm>


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

	// initialize the minimum and maximum values for the coordinates x, y, z
	this->xBounds[0] = INFINITY; // minimum
	this->xBounds[1] = -INFINITY; // maximum
	this->yBounds[0] = INFINITY; // minimum
	this->yBounds[1] = -INFINITY; // maximum
	this->zBounds[0] = INFINITY; // minimum
	this->zBounds[1] = -INFINITY; // maximum


	// Iterate through all geometry in the scene
	for (unsigned int i = 0; i < geometry->size(); i++)
	{
		// The BTreeAccelerator was designed for triangles.
		// Try parsing the geometry to a triangle.
		// If this fails, throw a custom exception
		try
		{
			TriangleGeometry* myTriangle;
			myTriangle = (TriangleGeometry*)geometry->at(i).get();

			// add the triangle to the btrees
			this->xTree->AddNode(geometry->at(i));
			this->yTree->AddNode(geometry->at(i));
			this->zTree->AddNode(geometry->at(i));

			// if the minimum/maximum-values for the x, y or z coordinate are smaller/larger
			// than those known, store them.
			Vec3Df vec0 = myTriangle->getVertex0();
			Vec3Df vec1 = myTriangle->getVertex1();
			Vec3Df vec2 = myTriangle->getVertex2();

			//take the smallest x, y and z as minimum bounds
			this->xBounds[0] = std::min(this->xBounds[0], std::min(vec0[0], std::min(vec1[0], vec2[0])));
			this->yBounds[0] = std::min(this->yBounds[0], std::min(vec0[1], std::min(vec1[1], vec2[1])));
			this->zBounds[0] = std::min(this->zBounds[0], std::min(vec0[2], std::min(vec1[2], vec2[2])));

			// take the largest x, y and z as maximum bounds
			this->xBounds[1] = std::max(this->xBounds[1], std::max(vec0[0], std::max(vec1[0], vec2[0])));
			this->yBounds[1] = std::max(this->yBounds[1], std::max(vec0[1], std::max(vec1[1], vec2[1])));
			this->zBounds[1] = std::max(this->zBounds[1], std::max(vec0[2], std::max(vec1[2], vec2[2])));
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
	std::vector<std::shared_ptr<IGeometry>> intersec1;
	std::set_intersection(xTriangles.begin(), xTriangles.end(), yTriangles.begin(), yTriangles.end(), intersec1.begin());
	//
	std::vector<std::shared_ptr<IGeometry>> myIntersection;
	std::set_intersection(intersec1.begin(), intersec1.end(), zTriangles.begin(), zTriangles.end(), myIntersection.begin());

	// feed the triangles to the ray-intersection algorithm


	// Not the most elegant solution but it should work
	Vec3Df dest = dir * 10000.0f;

	// Use the retrieveTriangles to find a list of potential intersecting objects
	std::vector<std::shared_ptr<IGeometry>> candidateSet = this->retrieveTriangles(origin, dest);

	// Constructor a NoAccelerationStructure to perform a simple intersection test
	NoAccelerationStructure simpleIntersectionAlgorithm;
	simpleIntersectionAlgorithm.setGeometry(std::make_shared<const std::vector<std::shared_ptr<IGeometry>>>(myIntersection));
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
		coord2Value = destination[2];
		coord2Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Y);

		coord3Value = destination[3];
		coord3Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Z);

		break;
	case BTree::Coordinate::Y:
		coord2Value = destination[1];
		coord2Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::X);

		coord3Value = destination[3];
		coord3Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::Z);


		break;
	case BTree::Coordinate::Z:
		coord2Value = destination[1];
		coord2Extreme = this->GetCoordinateExtreme(origin, destination, BTree::Coordinate::X);

		coord3Value = destination[2];
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