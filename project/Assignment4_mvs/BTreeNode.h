#ifndef BTREENODE_H
#define BTREENODE_H

#include <memory>

#include "BTree.h"

class IGeometry;

class BTreeNode
{
private:

	std::shared_ptr<IGeometry> data;
	int depth;

	BTree::Coordinate _coordinate;

	BTreeNode* parent;
	BTreeNode* leftChild;
	BTreeNode* rightChild;

	float GetAverageTriangleValue(std::shared_ptr<IGeometry> triangle) const;

public:
	BTreeNode(BTreeNode* parent, std::shared_ptr<IGeometry> tData, BTree::Coordinate coordinate);

	void AddNode(std::shared_ptr<IGeometry> data);

	int Compare(std::shared_ptr<IGeometry> data) const;

	int withinLimit(float lowerLimit, float upperLimit) const;

	void GetTriangles(std::vector<std::shared_ptr<IGeometry>> &collection, float lowerLimit, float upperLimit) const;

	void Balance();
};



class BTreeException : public std::exception
{
private:
	const char* _message;

public:

	BTreeException(const char* message);

	virtual const char* what() const throw();


};




#endif