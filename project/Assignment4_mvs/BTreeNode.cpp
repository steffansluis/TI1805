#include <algorithm>

#include "BTreeNode.h"

// @Author: Bas Boellaard
// get the average value for the indicated triangle with the current coordination
float BTreeNode::GetAverageTriangleValue(Triangle* triangle)
{
	int coordIndex;

	switch (this->_coordinate)
	{
	case BTree::Coordinate::X:
		coordIndex = 0;
		break;
	case BTree::Coordinate::Y:
		coordIndex = 1;
		break;
	case BTree::Coordinate::Z:
		coordIndex = 2;
		break;

	default:
		return 0;
	}

	float value = 0.0f;
	/*
	!!! TEMPORARY
	value += MyMesh.vertices[triangle->v[0]].p[coordIndex];
	value += MyMesh.vertices[triangle->v[1]].p[coordIndex];
	value += MyMesh.vertices[triangle->v[2]].p[coordIndex];
	*/

	return value / 3.0f;
}


// @Author: Bas Boellaard
// Create a new Node for the BTree. 
BTreeNode::BTreeNode(BTreeNode* tParent, Triangle* tData, BTree::Coordinate coordinate)
{
	// set depth to '0' (no children)
	BTreeNode::depth = 0;
	this->_coordinate = coordinate;
	this->data = tData;

	this->parent = tParent;

	this->leftChild = NULL;
	this->rightChild = NULL;
}


// @Author: Bas Boellaard
// Add a new node to the B-Tree. The triangle will either be stored as a new node as one 
// of the childs of this node, or it will be passed on to his childs. 
void BTreeNode::AddNode(Triangle* data)
{
	// first, compare the new triangle with our current triangle.
	int comparison = Compare(data);

	// if it is smaller (or equal):
	if (comparison <= 0)
	{
		// if the left child exists, recursively make him add
		if (this->leftChild != NULL)
		{
			this->leftChild->AddNode(data);
		}
		else
		{
			// since the left child does not exist, add this as the left child. 
			this->leftChild = &BTreeNode(this, data, this->_coordinate);

			// Balance ourselves out, if necessary
			this->Balance();
		}
	}
	else
	{
		// if the right child exists, recursively make him add
		if (this->rightChild != NULL)
		{
			this->rightChild->AddNode(data);
		}
		else
		{
			// since the right child does not exist, add this as the right child
			this->rightChild = &BTreeNode(this, data, this->_coordinate);

			// balance ourselves out, if necessary
			this->Balance();
		}
	}
}


// @Author: Bas Boellaard
// Compares a triangle with the triangle of the current node
// Outputs -1 if the other triangle is smaller, 0 if they are equal and 1 if it is larger.
int BTreeNode::Compare(Triangle* tData)
{
	// since the comparison is represented with 3 values, we take the average of those.
	float myValue = this->GetAverageTriangleValue(this->data);
	float hisValue = this->GetAverageTriangleValue(tData);

	if (hisValue < myValue)
	{
		return -1;
	}
	else if (hisValue > myValue)
	{
		return 1;
	}

	return 0;
}


// @Author: Bas Boellaard
// This method checks if the current triangle is within the set limits for the set coordinate.
// there are 3 possible outcomes:
// result = 1   : the upperLimit is smaller than the smallest proper coordinate of the triangle. (coordinates too large)
// result = 0   : the triangle is within the limits.
// result = -1  : the lowerLimit is larger than the largest proper coordinate of the triangle. (coordinates too small)
int BTreeNode::withinLimit(float lowerLimit, float upperLimit)
{
	int coordIndex;

	switch (this->_coordinate)
	{
	case BTree::Coordinate::X:
		coordIndex = 0;
		break;
	case BTree::Coordinate::Y:
		coordIndex = 1;
		break;
	case BTree::Coordinate::Z:
		coordIndex = 2;
		break;

	default:
		return 0;
	}

	// get the proper coordinates
	// !!! TEMPORARY
	float coord1 = 0;// MyMesh.vertices[this->data->v[0]].p[coordIndex];
	float coord2 = 0;// MyMesh.vertices[this->data->v[1]].p[coordIndex];
	float coord3 = 0;// MyMesh.vertices[this->data->v[2]].p[coordIndex];

	// check if they are less than than the lowerLimit
	if (coord1 < lowerLimit && coord2 < lowerLimit && coord3 < lowerLimit)
	{
		// this triangle is smaller than the lower limit.
		return -1;
	}

	// check if they are larger than the upperLimit
	if (coord1 > upperLimit && coord2 > upperLimit && coord3 > upperLimit)
	{
		// this triangle is larger than the upper limit.
		return 1;
	}

	// since neither was the case, this triangle is within the hit range
	return 0;
}


// @Author: Bas Boellaard
// Retrieve the triangles that are within the bounds of the limits. 
// If the current coordinates are too small (or within bounds), the right child (with larger coordinates) of the node is called.
// If the current coordinates are too large (or within bounds), the left child (with smaller coordinates) of the node is called.
// If the triangle of the current node is within bounds, it gets added to the collection. 
void BTreeNode::GetTriangles(std::vector<Triangle*> * collection, float lowerLimit, float upperLimit)
{
	// check if the current triangle is within limits
	int withinLimits = withinLimit(lowerLimit, upperLimit);

	// if the limit is more than (or equal to) 0, we can check the left childs.
	// this means that the triangles we need have a coordinate smaller than this current coordinate.
	if (withinLimits >= 0 && this->leftChild != NULL)
	{
		this->leftChild->GetTriangles(collection, lowerLimit, upperLimit);
	}

	// if the limit is les than (or equal to) 0, we can check the right childs.
	// this means that the triangles we need have a coordinate larger than this current coordinate.
	if (withinLimits <= 0 && this->rightChild != NULL)
	{
		this->rightChild->GetTriangles(collection, lowerLimit, upperLimit);
	}

	// if however withinLimits equals 0, that means the current triangle was within bounds
	if (withinLimits == 0)
	{
		collection->push_back(this->data);
	}
}


// @Author: Bas Boellaard
// balance out the node if necessary. 
// This method will also set the proper depth per node. 
// If not, call the parent node to balance.
void BTreeNode::Balance()
{
	int leftDepth = -1;
	int rightDepth = -1;

	if (this->leftChild != NULL)
	{
		leftDepth = BTreeNode::leftChild->depth;
	}

	if (this->rightChild != NULL)
	{
		rightDepth = BTreeNode::rightChild->depth;
	}

	// first, set our own depth properly
	this->depth = std::max<int>(leftDepth + 1, rightDepth + 1);


	// if the difference in depth is more than 3, it requires balancing.
	if (rightDepth - leftDepth >= 3)
	{
		// The right part of the node is too large.
		// assume L = left child ; T = current node (top) ; R = right child
		// The following changes must be made:
		// - the parent of T becomes the parent of R
		// - the child of the parent of T changes appropriately
		// - R becomes the parent of T
		// - The right child of T becomes the left child of R
		// - the left child of R becomes T
		// - the depth of T becomes the depth of L + 1
		//
		// store R in advance
		BTreeNode* R = this->rightChild;

		R->parent = this->parent;

		if (R->parent->leftChild == this)
		{
			R->parent->leftChild = R;
		}
		else if (R->parent->rightChild == this)
		{
			R->parent->rightChild = R;
		}
		else
		{
			std::cout << "something went seriously wrong!!! BTreeNode::Balance()";
		}

		this->parent = R;
		this->rightChild = R->leftChild;
		R->leftChild = this;
		this->depth = this->leftChild->depth + 1;
	}
	else if (leftDepth - rightDepth >= 3)
	{
		// The left part of the node is too large.
		// assume L = left child ; T = current node (top) ; R = right child
		// The following changes must be made:
		// - the parent of T becomes the parent of L
		// - the child of the parents of T changes appropriately
		// - L becomes the parent of T
		// - the left child of T becomes the right child of L
		// - the right child of L becomes T
		// the depth of T becomes the depth of R + 1
		//
		// store L in advance
		BTreeNode* L = this->leftChild;

		L->parent = this->parent;

		if (L->parent->leftChild == this)
		{
			L->parent->leftChild = L;
		}
		else if (L->parent->rightChild == this)
		{
			L->parent->rightChild = L;
		}
		else
		{
			std::cout << "something went seriously wrong!!! BTreeNode::Balance()";
		}

		this->parent = L;
		this->leftChild = L->rightChild;
		L->rightChild = this;
		this->depth = this->rightChild->depth + 1;
	}

	// if the tree was balanced, there is no need to set any other depths. 
	// This will be done with the recursive Balance() call to the parent nodes.
	//
	// if your parent is not null, balance him out.
	if (this->parent != NULL)
	{
		this->parent->Balance();
	}
}