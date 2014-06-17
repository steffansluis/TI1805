#ifndef BTREE_H
#define BTREE_H

#include <memory>
#include <vector>

class BTreeNode;
class IGeometry;

class BTree
{
public:
	enum class Coordinate { X, Y, Z };

private:
	BTreeNode* head;

	Coordinate _coordinate;

public:

	BTree(Coordinate coordinate);

	void AddNode(std::shared_ptr<IGeometry> data);

	std::vector<std::shared_ptr<IGeometry>> GetTriangles(float lowerLimit, float upperLimit) const;
};

#endif