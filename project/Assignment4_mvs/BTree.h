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
	std::shared_ptr<BTreeNode> head;

	Coordinate _coordinate;

public:

	BTree(Coordinate coordinate);

	void AddNode(std::shared_ptr<IGeometry> data);

	std::vector<std::shared_ptr<IGeometry>> GetTriangles(const float limit1, const float limit2) const;

	void SetHeadNode(std::shared_ptr<BTreeNode> headNode);

	std::shared_ptr<BTreeNode> GetHeadNode() const;

};

#endif