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
	if (BTree::head == NULL)
	{
		BTree::head = new BTreeNode(NULL, data, this->_coordinate);
	}
	else
	{
		BTree::head->AddNode(data);
	}
}


// @Author: Bas Boellaard
// Returns the triangles that are within bounds of the arguments. 
// These bounds are applied to the coordinate that this B-Tree sorts on. 
std::vector<std::shared_ptr<IGeometry>> BTree::GetTriangles(float lowerLimit, float upperLimit) const
{
	auto output = std::vector<std::shared_ptr<IGeometry>>();

	if (this->head != NULL)
	{
		this->head->GetTriangles(output, lowerLimit, upperLimit);
	}

	return output;
}