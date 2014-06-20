#include <algorithm>
#include <exception>

#include "BoundingBox.h"
#include "BTreeNode.h"
#include "IGeometry.h"

// @Author: Bas Boellaard
// get the average value for the indicated triangle with the current coordination
float BTreeNode::GetAverageTriangleValue(std::shared_ptr<IGeometry> triangle) const
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

	// Get the center of the boundig box of the geometry
	Vec3Df average = triangle->getBoundingBox().getCenter();

	// Return the coordIndex component
	return average[coordIndex];
}


// @Author: Bas Boellaard
// Create a new Node for the BTree. 
BTreeNode::BTreeNode(BTree* master, std::shared_ptr<BTreeNode> tParent, std::shared_ptr<IGeometry> tData, BTree::Coordinate coordinate)
{
	// set the BTree owner
	this->myMaster = master;

	// set depth to '0' (no children)
	BTreeNode::depth = 0;
	this->_coordinate = coordinate;
	this->data = tData;

	this->parent = tParent;

	//this->leftChild = std::shared_ptr<BTreeNode>();
	//this->rightChild = NULL;
}


void BTreeNode::SetMyself(std::shared_ptr<BTreeNode> myself)
{
	this->_myself = myself;
}



// @Author: Bas Boellaard
// Add a new node to the B-Tree. The triangle will either be stored as a new node as one 
// of the childs of this node, or it will be passed on to his childs. 
void BTreeNode::AddNode(std::shared_ptr<IGeometry> data)
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
			this->leftChild = std::make_shared<BTreeNode>(this->myMaster, this->_myself, data, this->_coordinate);
			this->leftChild->SetMyself(this->leftChild);

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
			this->rightChild = std::make_shared<BTreeNode>(this->myMaster, this->_myself, data, this->_coordinate);
			this->rightChild->SetMyself(this->rightChild);

			// balance ourselves out, if necessary
			this->Balance();
		}
	}
}


// @Author: Bas Boellaard
// Compares a triangle with the triangle of the current node
// Outputs -1 if the other triangle is smaller, 0 if they are equal and 1 if it is larger.
int BTreeNode::Compare(std::shared_ptr<IGeometry> tData) const
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
int BTreeNode::withinLimit(float lowerLimit, float upperLimit) const
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
	BoundingBox boundingBox = this->data->getBoundingBox();
	float min = boundingBox.min[coordIndex];
	float max = boundingBox.max[coordIndex];

	// check if they are less than than the lowerLimit
	if (max < lowerLimit)
	{
		// this triangle is smaller than the lower limit.
		return -1;
	}

	// check if they are larger than the upperLimit
	if (min > upperLimit)
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
void BTreeNode::GetTriangles(std::vector<std::shared_ptr<IGeometry>> &collection, float lowerLimit, float upperLimit) const
{
	// check if the current triangle is within limits
	int withinLimits = withinLimit(lowerLimit, upperLimit);

	// if the limit is more than (or equal to) 0, we can check the left childs.
	// this means that the triangles we need have a coordinate smaller than this current coordinate.
	if (withinLimits >= 0 && this->leftChild != NULL)
	{
		this->leftChild->GetTriangles(collection, lowerLimit, upperLimit);
	}

	// if withinLimits equals 0, that means the current triangle was within bounds
	if (withinLimits == 0)
	{
		collection.push_back(this->data);
	}

	// if the limit is les than (or equal to) 0, we can check the right childs.
	// this means that the triangles we need have a coordinate larger than this current coordinate.
	if (withinLimits <= 0 && this->rightChild != NULL)
	{
		this->rightChild->GetTriangles(collection, lowerLimit, upperLimit);
	}
}


// @Author: Bas Boellaard
// balance out the node if necessary. 
// This method will also set the proper depth per node. 
// If not, call the parent node to balance.
void BTreeNode::Balance()
{
	this->DetermineDepth();

	int leftDepth = -1;
	int rightDepth = -1;

	if (this->leftChild != NULL)
	{
		leftDepth = this->leftChild->GetDepth();
	}

	if (this->rightChild != NULL)
	{
		rightDepth = this->rightChild->GetDepth();
	}


	// if the difference in depth is more than 3, it requires balancing.
	if (this->depth - leftDepth >= 3)
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
		std::shared_ptr<BTreeNode> R = this->rightChild;
		
		R->SetParent(this->parent);

		// if this is the headnode, assign R as the new head
		if (this->parent == NULL)
		{
			this->myMaster->SetHeadNode(R);
		}
		else
		{
			if (R->GetParent()->GetLeftChild().get() == this)
			{
				R->GetParent()->SetLeftChild(R);
			}
			else if (R->GetParent()->GetRightChild().get() == this)
			{
				R->GetParent()->SetRightChild(R);
			}
			else
			{
				throw BTreeException("This node is an orphan!!");
			}
		}

		this->parent = R;
		this->rightChild = R->GetLeftChild();
		R->SetLeftChild(this->_myself);
		this->DetermineDepth();
	}
	else if (this->depth - rightDepth >= 3)
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
		std::shared_ptr<BTreeNode> L = this->leftChild;

		L->SetParent(this->parent);

		// if this is the headnode, assign L as the new head
		if (this->parent == NULL)
		{
			this->myMaster->SetHeadNode(L);
		}
		else
		{
			if (L->GetParent()->GetLeftChild().get() == this)
			{
				L->GetParent()->SetLeftChild(L);
			}
			else if (L->GetParent()->GetRightChild().get() == this)
			{
				L->GetParent()->SetRightChild(L);
			}
			else
			{
				throw BTreeException("This node is an orphan!!");
			}
		}

		this->parent = L;
		this->leftChild = L->GetRightChild();
		L->SetRightChild(this->_myself);
		this->DetermineDepth();
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


void BTreeNode::DetermineDepth()
{
	int leftDepth = -1;
	int rightDepth = -1;

	if (this->leftChild != NULL)
	{
		leftDepth = this->leftChild->GetDepth();
	}

	if (this->rightChild != NULL)
	{
		rightDepth = this->rightChild->GetDepth();
	}

	// set our own depth properly
	this->depth = std::max<int>(leftDepth + 1, rightDepth + 1);
}






////////////////////// Data Methods /////////////////////

void BTreeNode::SetParent(std::shared_ptr<BTreeNode> newParent)
{
	this->parent = newParent;
}

std::shared_ptr<BTreeNode> BTreeNode::GetParent() const
{
	return this->parent;
}


void BTreeNode::SetLeftChild(std::shared_ptr<BTreeNode> newLeftChild)
{
	this->leftChild = newLeftChild;
}

std::shared_ptr<BTreeNode> BTreeNode::GetLeftChild() const
{
	return this->leftChild;
}


void BTreeNode::SetRightChild(std::shared_ptr<BTreeNode> newRightChild)
{
	this->rightChild = newRightChild;
}

std::shared_ptr<BTreeNode> BTreeNode::GetRightChild() const
{
	return this->rightChild;
}



void BTreeNode::SetDepth(int newDepth)
{
	this->depth = newDepth;
}

int BTreeNode::GetDepth() const
{
	return this->depth;
}









////////////////// Custom Exceptions ////////////////////////

BTreeException::BTreeException(const char* message)
{
	this->_message = message;
}

const char* BTreeException::what() const throw()
{
	return this->_message;
}


