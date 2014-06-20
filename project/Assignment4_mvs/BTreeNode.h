#ifndef BTREENODE_H
#define BTREENODE_H

#include <memory>

#include "BTree.h"

class IGeometry;

class BTreeNode
{
private:

	BTree* myMaster;

	std::shared_ptr<IGeometry> data;
	int depth;

	BTree::Coordinate _coordinate;

	std::shared_ptr<BTreeNode> parent;
	std::shared_ptr<BTreeNode> leftChild;
	std::shared_ptr<BTreeNode> rightChild;

	std::shared_ptr<BTreeNode> _myself;

	float GetAverageTriangleValue(std::shared_ptr<IGeometry> triangle) const;

public:
	BTreeNode(BTree* master, std::shared_ptr<BTreeNode> parent, std::shared_ptr<IGeometry> tData, BTree::Coordinate coordinate);

	void SetMyself(std::shared_ptr<BTreeNode> myself);

	void AddNode(std::shared_ptr<IGeometry> data);

	int Compare(std::shared_ptr<IGeometry> data) const;

	int withinLimit(float lowerLimit, float upperLimit) const;

	void GetTriangles(std::vector<std::shared_ptr<IGeometry>> &collection, float lowerLimit, float upperLimit) const;


	// Balancing methods
	void Balance();
	void DetermineDepth();


	// Data methods
	void SetParent(std::shared_ptr<BTreeNode> newParent);
	std::shared_ptr<BTreeNode> GetParent() const;

	void SetLeftChild(std::shared_ptr<BTreeNode> newLeftChild);
	std::shared_ptr<BTreeNode> GetLeftChild() const;

	void SetRightChild(std::shared_ptr<BTreeNode> newRightChild);
	std::shared_ptr<BTreeNode> GetRightChild() const;

	void SetDepth(int newDepth);
	int GetDepth() const;



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