#ifndef BTREE_H
#define BTREE_H

#include <vector>

#include "mesh.h"

class BTreeNode;

class BTree
{
public:
	enum class Coordinate { X, Y, Z };

private:
	BTreeNode* head;

	Coordinate _coordinate;

public:

	BTree(Coordinate coordinate);

	void AddNode(Triangle* data);

	std::vector<Triangle*> & GetTriangles(float lowerLimit, float upperLimit);
};

#endif