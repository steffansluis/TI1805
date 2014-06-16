#ifndef BTREENODE_H
#define BTREENODE_H

#include "BTree.h"
#include "mesh.h"

class BTreeNode
{
private:

	Triangle* data;
	int depth;

	BTree::Coordinate _coordinate;

	BTreeNode* parent;
	BTreeNode* leftChild;
	BTreeNode* rightChild;

	float GetAverageTriangleValue(Triangle* triangle);

public:
	BTreeNode(BTreeNode* parent, Triangle* tData, BTree::Coordinate coordinate);

	void AddNode(Triangle* data);

	int Compare(Triangle* data);

	int withinLimit(float lowerLimit, float upperLimit);

	void GetTriangles(std::vector<Triangle*> * collection, float lowerLimit, float upperLimit);

	void Balance();
};

#endif