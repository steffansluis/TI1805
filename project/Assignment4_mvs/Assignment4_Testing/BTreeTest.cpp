#include "..\BTree.h"
#include "..\BTreeNode.h"
#include "..\TriangleGeometry.h"

#include <vector>
#include <memory>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Assignment4_Testing
{
	[TestClass]
	public ref class BTreeTest
	{
	private:
		std::vector<std::shared_ptr<IGeometry>> *triangles;

		std::vector<std::shared_ptr<BTree>>* bTrees;
		

	public:

		[TestInitialize()]
		void Before()
		{
			this->bTrees = new std::vector<std::shared_ptr<BTree>>();

			// initialize B-Trees for all coordinations
			this->bTrees->push_back(std::make_shared<BTree>(BTree::Coordinate::X));
			this->bTrees->push_back(std::make_shared<BTree>(BTree::Coordinate::Y));
			this->bTrees->push_back(std::make_shared<BTree>(BTree::Coordinate::Z));

			// allocate a pointer to a pointer
			this->triangles = new std::vector<std::shared_ptr<IGeometry>>();

			// initialize the triangle vector as a shared pointer
			//*this->triangles = std::vector<std::shared_ptr<IGeometry>>>();

			// add 5 different triangles
			this->triangles->push_back(std::make_shared<TriangleGeometry>(
				Vec3Df(0, 0, 0),
				Vec3Df(1, 1, 1),
				Vec3Df(2, 2, 2)));

			this->triangles->push_back(std::make_shared<TriangleGeometry>(
				Vec3Df(1, 1, 1),
				Vec3Df(2, 2, 2), 
				Vec3Df(3, 3, 3)));

			this->triangles->push_back(std::make_shared<TriangleGeometry>(
				Vec3Df(2, 2, 2),
				Vec3Df(3, 3, 3),	
				Vec3Df(4, 4, 4)));

			this->triangles->push_back(std::make_shared<TriangleGeometry>(
				Vec3Df(3, 3, 3),
				Vec3Df(4, 4, 4), 
				Vec3Df(5, 5, 5)));

			this->triangles->push_back(std::make_shared<TriangleGeometry>(
				Vec3Df(4, 4, 4),
				Vec3Df(5, 5, 5), 
				Vec3Df(6, 6, 6)));

		}

		[TestCleanup()]
		void After()	
		{
			delete this->bTrees;
			delete this->triangles;
		}


		[TestMethod]
		void testBTreeEmpty()
		{
			// for all bTrees
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// get the triangles for the empty bTree
				std::vector<std::shared_ptr<IGeometry>> xTriangles = bTree->GetTriangles(-INFINITY, INFINITY);

				// check whether it has the correct return size
				Assert::AreEqual<System::UInt32>(0, xTriangles.size());
			}
		}

		[TestMethod]
		void testBTreeSingleEntry()
		{
			// for all bTrees
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add the first entry
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				//
				bTree.get()->AddNode(triangle0);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(1, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// a single node has:
				// no parent
				Assert::IsTrue(NULL == treeHeadNode->GetParent());
				//
				// no children
				Assert::IsTrue(NULL == treeHeadNode->GetLeftChild());
				//
				Assert::IsTrue(NULL == treeHeadNode->GetRightChild());
				//
				// and a depth of 0
				Assert::AreEqual<System::UInt32>(0, treeHeadNode->GetDepth());
			}
		}

		[TestMethod]
		void testBTreeLeftChildAssignment()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add two entries; the second entry must be smaller than the first
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				//
				bTree->AddNode(triangle1);
				bTree->AddNode(triangle0);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(2, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 1, no parent and does have a left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				Assert::IsTrue(leftChild.get() != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetRightChild() == NULL);
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(1, treeHeadNode->GetDepth());
				//
				// the left child has a depth of 0, no children and the headnode as parent
				Assert::IsTrue(leftChild->GetLeftChild() == NULL);
				Assert::IsTrue(leftChild->GetRightChild() == NULL);
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, leftChild->GetDepth());
			}
		}
		[TestMethod]
		void testBTreeRightChildAssignment()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add two entries; the second entry must be larger than the first
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				//
				bTree->AddNode(triangle0);
				bTree->AddNode(triangle1);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(2, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 1, no parent and does have a right child
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetLeftChild() == NULL);
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(1, treeHeadNode->GetDepth());
				//
				// the left child has a depth of 0, no children and the headnode as parent
				Assert::IsTrue(rightChild->GetLeftChild() == NULL);
				Assert::IsTrue(rightChild->GetRightChild() == NULL);
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
			}
		}

		// @Author: Bas Boellaard
		// This method inserts 3 items, the latter two are each smaller than the other.
		// The expected behavior of the nodes is to balance out and create a singular
		// headnode with 2 childs.
		[TestMethod]
		void testBTreeBalanceLeft()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add three entries; the latter two must be smaller than the former values.
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				//
				bTree->AddNode(triangle2);
				bTree->AddNode(triangle1);
				bTree->AddNode(triangle0);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(3, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 1, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(1, treeHeadNode->GetDepth());
				//
				// the left child has a depth of 0, no children and the headnode as parent
				Assert::IsTrue(leftChild->GetLeftChild() == NULL);
				Assert::IsTrue(leftChild->GetRightChild() == NULL);
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, leftChild->GetDepth());
				// 
				// the right child has a depth of 0, no children and the headnode as parent
				Assert::IsTrue(rightChild->GetLeftChild() == NULL);
				Assert::IsTrue(rightChild->GetRightChild() == NULL);
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
			}
		}

		// @Author: Bas Boellaard
		// This method inserts 3 items, the latter two are each larger than the other. 
		// The expected behavior of the nodes is to balance out and create a singular headnode 
		// with 2 childs.
		[TestMethod]
		void testBTreeBalanceRight()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add three entries; the latter two must be larger than the former values.
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				//
				bTree->AddNode(triangle0);
				bTree->AddNode(triangle1);
				bTree->AddNode(triangle2);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(3, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 1, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(1, treeHeadNode->GetDepth());
				//
				// the left child has a depth of 0, no children and the headnode as parent
				Assert::IsTrue(leftChild->GetLeftChild() == NULL);
				Assert::IsTrue(leftChild->GetRightChild() == NULL);
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, leftChild->GetDepth());
				// 
				// the right child has a depth of 0, no children and the headnode as parent
				Assert::IsTrue(rightChild->GetLeftChild() == NULL);
				Assert::IsTrue(rightChild->GetRightChild() == NULL);
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
			}
		}

		// @Author: Bas Boellaard
		// In order to test the left-left child, we need to prevent the tree from ordering.
		// this means that for values 0 - 1 - 2 - 3 (sorted) they need to be insert in the following order:
		// 2 (head)
		// 1 + 3 (left + right)
		// 0 (left-left)
		[TestMethod]
		void testBTreeLeftLeftChildAssignment()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add 4 entries
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				std::shared_ptr<IGeometry> triangle3 = this->GetTriangle(3);
				//
				bTree->AddNode(triangle2);
				bTree->AddNode(triangle1);
				bTree->AddNode(triangle3);
				bTree->AddNode(triangle0);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(4, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));
				Assert::IsTrue(AreEqualTriangles(triangle3.get(), result[3].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 2, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(2, treeHeadNode->GetDepth());
				//
				// The first rightnode has a depth of 0 and no children and the headnode as parent
				Assert::IsTrue(HasNoChildren(rightChild));
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
				//
				// the first leftnode has a depth of 1, a left child, no right child and the 
				// headnode as parent
				std::shared_ptr<BTreeNode> leftLeftChild = leftChild->GetLeftChild();
				Assert::IsTrue(leftLeftChild != NULL);
				//
				Assert::IsTrue(leftChild->GetRightChild() == NULL);
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(1, leftChild->GetDepth());
				//
				// The left-left child has a depth of 0, no children and the leftnode as parent
				Assert::IsTrue(HasNoChildren(leftLeftChild));
				Assert::IsTrue(leftLeftChild->GetParent() == leftChild);
				Assert::AreEqual(0, leftLeftChild->GetDepth());
			}
		}


		// @Author: Bas Boellaard
		// In order to test the left-right child, we need to insert items in the following order:
		// 2 (head)
		// 0 + 3 (left + right)
		// 1 (left-right)
		[TestMethod]
		void testBTreeLeftRightChildAssignment()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add 4 entries
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				std::shared_ptr<IGeometry> triangle3 = this->GetTriangle(3);
				//
				bTree->AddNode(triangle2);
				bTree->AddNode(triangle0);
				bTree->AddNode(triangle3);
				bTree->AddNode(triangle1);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(4, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));
				Assert::IsTrue(AreEqualTriangles(triangle3.get(), result[3].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 2, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(2, treeHeadNode->GetDepth());
				//
				// The first rightnode has a depth of 0 and no children and the headnode as parent
				Assert::IsTrue(HasNoChildren(rightChild));
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
				//
				// the first leftnode has a depth of 1, a right child, no left child and the 
				// headnode as parent
				std::shared_ptr<BTreeNode> leftRightChild = leftChild->GetRightChild();
				Assert::IsTrue(leftRightChild != NULL);
				//
				Assert::IsTrue(leftChild->GetLeftChild() == NULL);
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(1, leftChild->GetDepth());
				//
				// The left-right child has a depth of 0, no children and the leftnode as parent
				Assert::IsTrue(HasNoChildren(leftRightChild));
				Assert::IsTrue(leftRightChild->GetParent() == leftChild);
				Assert::AreEqual(0, leftRightChild->GetDepth());
			}
		}

		// @Author: Bas Boellaard
		// In order to test the right-left child, we need to insert items in the following order:
		// 1 (head)
		// 0 + 3 (left + right)
		// 2 (left-right)
		[TestMethod]
		void testBTreeRightLeftChildAssignment()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add 4 entries
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				std::shared_ptr<IGeometry> triangle3 = this->GetTriangle(3);
				//
				bTree->AddNode(triangle1);
				bTree->AddNode(triangle0);
				bTree->AddNode(triangle3);
				bTree->AddNode(triangle2);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(4, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));
				Assert::IsTrue(AreEqualTriangles(triangle3.get(), result[3].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 2, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(2, treeHeadNode->GetDepth());
				//
				// The first rightnode has a depth of 1, a left child, no right child and 
				// the headnode as parent
				std::shared_ptr<BTreeNode> rightLeftChild = rightChild->GetLeftChild();
				Assert::IsTrue(rightLeftChild != NULL);
				//
				Assert::IsTrue(rightChild->GetRightChild() == NULL);
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(1, rightChild->GetDepth());
				//
				// the right-left child has a depth of 0, no children and the rightnode as parent
				Assert::IsTrue(HasNoChildren(rightLeftChild));
				Assert::IsTrue(rightLeftChild->GetParent() == rightChild);
				Assert::AreEqual(0, rightLeftChild->GetDepth());
				// 
				// the first leftnode has a depth of 0, no children and the 
				// headnode as parent
				Assert::IsTrue(HasNoChildren(leftChild));
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, leftChild->GetDepth());
			}
		}

		// @Author: Bas Boellaard
		// In order to test the right-right child, we need to insert items in the following order:
		// 1 (head)
		// 0 + 2 (left + right)
		// 3 (left-right)
		[TestMethod]
		void testBTreeRightRightChildAssignment()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add 4 entries
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				std::shared_ptr<IGeometry> triangle3 = this->GetTriangle(3);
				//
				bTree->AddNode(triangle1);
				bTree->AddNode(triangle0);
				bTree->AddNode(triangle2);
				bTree->AddNode(triangle3);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(4, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));
				Assert::IsTrue(AreEqualTriangles(triangle3.get(), result[3].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 2, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(2, treeHeadNode->GetDepth());
				//
				// The first rightnode has a depth of 1, a right child, no left child and 
				// the headnode as parent
				std::shared_ptr<BTreeNode> rightRightChild = rightChild->GetRightChild();
				Assert::IsTrue(rightRightChild != NULL);
				//
				Assert::IsTrue(rightChild->GetLeftChild() == NULL);
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(1, rightChild->GetDepth());
				//
				// the right-left child has a depth of 0, no children and the rightnode as parent
				Assert::IsTrue(HasNoChildren(rightRightChild));
				Assert::IsTrue(rightRightChild->GetParent() == rightChild);
				Assert::AreEqual(0, rightRightChild->GetDepth());
				// 
				// the first leftnode has a depth of 0, no children and the 
				// headnode as parent
				Assert::IsTrue(HasNoChildren(leftChild));
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, leftChild->GetDepth());
			}
		}


		// @Author: Bas Boellaard
		// This method inserts 5 items in the following order:
		// 3 (head)
		// 0 + 4 (left + right)
		// 1 (left-right)
		// 2 (left-right-right)
		// The expected behavior of the nodes is to balance out the nodes 0, 1 and 2. 
		[TestMethod]
		void testBTreeBalanceSubnodeRight()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add three entries; the latter two must be larger than the former values.
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				std::shared_ptr<IGeometry> triangle3 = this->GetTriangle(3);
				std::shared_ptr<IGeometry> triangle4 = this->GetTriangle(4);
				//
				bTree->AddNode(triangle3);
				bTree->AddNode(triangle0);
				bTree->AddNode(triangle4);
				bTree->AddNode(triangle1);
				// the following node should trigger a balance
				bTree->AddNode(triangle2);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(5, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));
				Assert::IsTrue(AreEqualTriangles(triangle3.get(), result[3].get()));
				Assert::IsTrue(AreEqualTriangles(triangle4.get(), result[4].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 2, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(2, treeHeadNode->GetDepth());
				//
				// the left child has a depth of 1, both children and the headnode as parent
				std::shared_ptr<BTreeNode> leftLeftChild = leftChild->GetLeftChild();
				std::shared_ptr<BTreeNode> leftRightChild = leftChild->GetRightChild();
				Assert::IsTrue(leftLeftChild != NULL);
				Assert::IsTrue(leftRightChild != NULL);
				//
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(1, leftChild->GetDepth());
				//
				// the left-left child has no children, the leftChild as parent and a depth of 0
				Assert::IsTrue(HasNoChildren(leftLeftChild));
				Assert::IsTrue(leftLeftChild->GetParent() == leftChild);
				Assert::AreEqual(0, leftLeftChild->GetDepth());
				//
				// the left-right child has no children, the leftChild as parent and a depth of 0
				Assert::IsTrue(HasNoChildren(leftRightChild));
				Assert::IsTrue(leftRightChild->GetParent() == leftChild);
				Assert::AreEqual(0, leftRightChild->GetDepth());
				//
				// The rightChild has no children, the headnode as parent and a depth of 0
				Assert::IsTrue(HasNoChildren(rightChild));
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
			}
		}

		// @Author: Bas Boellaard
		// This method inserts 5 items in the following order:
		// 3 (head)
		// 2 + 4 (left + right)
		// 1 (left-left)
		// 0 (left-left-left)
		// The expected behavior of the nodes is to balance out the nodes 0, 1 and 2. 
		[TestMethod]
		void testBTreeBalanceSubnodeLeft()
		{
			for (unsigned int i = 0; i < this->bTrees->size(); i++)
			{
				std::shared_ptr<BTree> bTree = GetBTree(i);

				// add three entries; the latter two must be larger than the former values.
				std::shared_ptr<IGeometry> triangle0 = this->GetTriangle(0);
				std::shared_ptr<IGeometry> triangle1 = this->GetTriangle(1);
				std::shared_ptr<IGeometry> triangle2 = this->GetTriangle(2);
				std::shared_ptr<IGeometry> triangle3 = this->GetTriangle(3);
				std::shared_ptr<IGeometry> triangle4 = this->GetTriangle(4);
				//
				bTree->AddNode(triangle3);
				bTree->AddNode(triangle2);
				bTree->AddNode(triangle4);
				bTree->AddNode(triangle1);
				// the following node should trigger a balance
				bTree->AddNode(triangle0);

				// check if the returned result is correct
				std::vector<std::shared_ptr<IGeometry>> result = bTree->GetTriangles(-INFINITY, INFINITY);
				//
				Assert::AreEqual<System::UInt32>(5, result.size());
				//
				Assert::IsTrue(AreEqualTriangles(triangle0.get(), result[0].get()));
				Assert::IsTrue(AreEqualTriangles(triangle1.get(), result[1].get()));
				Assert::IsTrue(AreEqualTriangles(triangle2.get(), result[2].get()));
				Assert::IsTrue(AreEqualTriangles(triangle3.get(), result[3].get()));
				Assert::IsTrue(AreEqualTriangles(triangle4.get(), result[4].get()));

				// check for all trees if the nodes are set up properly
				std::shared_ptr<BTreeNode> treeHeadNode = bTree->GetHeadNode();
				//
				// the first headnode has a depth of 2, no parent and both a right and left child
				std::shared_ptr<BTreeNode> leftChild = treeHeadNode->GetLeftChild();
				std::shared_ptr<BTreeNode> rightChild = treeHeadNode->GetRightChild();
				Assert::IsTrue(leftChild != NULL);
				Assert::IsTrue(rightChild != NULL);
				//
				Assert::IsTrue(treeHeadNode->GetParent() == NULL);
				Assert::AreEqual(2, treeHeadNode->GetDepth());
				//
				// the left child has a depth of 1, both children and the headnode as parent
				std::shared_ptr<BTreeNode> leftLeftChild = leftChild->GetLeftChild();
				std::shared_ptr<BTreeNode> leftRightChild = leftChild->GetRightChild();
				Assert::IsTrue(leftLeftChild != NULL);
				Assert::IsTrue(leftRightChild != NULL);
				//
				Assert::IsTrue(leftChild->GetParent() == treeHeadNode);
				Assert::AreEqual(1, leftChild->GetDepth());
				//
				// the left-left child has no children, the leftChild as parent and a depth of 0
				Assert::IsTrue(HasNoChildren(leftLeftChild));
				Assert::IsTrue(leftLeftChild->GetParent() == leftChild);
				Assert::AreEqual(0, leftLeftChild->GetDepth());
				//
				// the left-right child has no children, the leftChild as parent and a depth of 0
				Assert::IsTrue(HasNoChildren(leftRightChild));
				Assert::IsTrue(leftRightChild->GetParent() == leftChild);
				Assert::AreEqual(0, leftRightChild->GetDepth());
				//
				// The rightChild has no children, the headnode as parent and a depth of 0
				Assert::IsTrue(HasNoChildren(rightChild));
				Assert::IsTrue(rightChild->GetParent() == treeHeadNode);
				Assert::AreEqual(0, rightChild->GetDepth());
			}
		}





		//////////// Helper methods /////////////////
		std::shared_ptr<IGeometry> GetTriangle(int index)
		{
			return (*this->triangles)[index];
		}

		
		std::shared_ptr<BTree> GetBTree(int index)
		{
			return (*this->bTrees)[index];
		}


		bool AreEqualTriangles(const IGeometry* first, const IGeometry* second)
		{
			TriangleGeometry* myFirst;
			TriangleGeometry* mySecond;

			myFirst = (TriangleGeometry*)first;
			mySecond = (TriangleGeometry*)second;

			Vec3Df firstVec0 = myFirst->getVertex0();
			Vec3Df firstVec1 = myFirst->getVertex1();
			Vec3Df firstVec2 = myFirst->getVertex2();

			Vec3Df secondVec0 = mySecond->getVertex0();
			Vec3Df secondVec1 = mySecond->getVertex1();
			Vec3Df secondVec2 = mySecond->getVertex2();

			return
				firstVec0[0] == secondVec0[0] &&
				firstVec0[1] == secondVec0[1] &&
				firstVec0[2] == secondVec0[2] &&

				firstVec1[0] == secondVec1[0] &&
				firstVec1[1] == secondVec1[1] &&
				firstVec1[2] == secondVec1[2] &&

				firstVec2[0] == secondVec2[0] &&
				firstVec2[1] == secondVec2[1] &&
				firstVec2[2] == secondVec2[2];
		}

		bool HasNoChildren(std::shared_ptr<BTreeNode> treeNode)
		{
			return
				treeNode->GetLeftChild() == NULL &&
				treeNode->GetRightChild() == NULL;
		}

	};
}
