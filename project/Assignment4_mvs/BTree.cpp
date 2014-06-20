#include "BTree.h"
#include "BTreeNode.h"
#include "IGeometry.h"

// @Author: Bas Boellaard
// Create a new B-Tree that sorts on the indicated coordinate.
BTree::BTree(Coordinate coordinate)
{
	this->_coordinate = coordinate;

	this->head = NULL;
}


// @Author: Bas Boellaard
// Add a new triangle to the B-Tree. If this is the first item, it will replace the head.
void BTree::AddNode(std::shared_ptr<IGeometry> data)
{
	if (this->head == NULL)
	{
		std::shared_ptr<BTreeNode> headpointer = std::make_shared<BTreeNode>(this, nullptr, data, this->_coordinate);
		headpointer->SetMyself(headpointer);
		this->head = headpointer;
	}
	else
	{
		this->head->AddNode(data);
	}
}


// @Author: Bas Boellaard
// Returns the triangles that are within bounds of the arguments. 
// These bounds are applied to the coordinate that this B-Tree sorts on. 
std::vector<std::shared_ptr<IGeometry>> BTree::GetTriangles(const float lowerLimit, const float upperLimit) const
{
	auto output = std::vector<std::shared_ptr<IGeometry>>();

	if (this->head != NULL)
	{
		this->head->GetTriangles(output, lowerLimit, upperLimit);
	}

	return output;
}


void BTree::SetHeadNode(std::shared_ptr<BTreeNode> headNode)
{
	this->head = headNode;
}


// @Author: Bas Boellaard
// This method is used for testing to ensure the proper tree structure.
std::shared_ptr<BTreeNode> BTree::GetHeadNode() const
{
	return this->head;
}