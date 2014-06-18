

#include "stdafx.h"

#include "..\BTree.h"
#include "..\TriangleGeometry.h"

#include <vector>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Assignment4_Testing
{
	[TestClass]
	public ref class UnitTest
	{
	private:
		std::shared_ptr<std::vector<IGeometry*>> * triangles;

		BTree* xTree;
		BTree* yTree;
		BTree* zTree;
		

	public:

		[TestInitialize()]
		void Before()
		{
			// initialize B-Trees for all coordinations
			this->xTree = &BTree(BTree::Coordinate::X);
			this->yTree = &BTree(BTree::Coordinate::Y);
			this->zTree = &BTree(BTree::Coordinate::Z);

			// initialize the triangle vector as a shared pointer
			this->triangles = &std::make_shared<std::vector<IGeometry*>>
				(std::vector<IGeometry*>());

			// add 5 different triangles
			this->triangles->get()->push_back(&TriangleGeometry(
				Vec3Df(0, 0, 0),
				Vec3Df(1, 1, 1),
				Vec3Df(2, 2, 2)));

			this->triangles->get()->push_back(&TriangleGeometry(
				Vec3Df(1, 1, 1),
				Vec3Df(2, 2, 2), 
				Vec3Df(3, 3, 3)));

			this->triangles->get()->push_back(&TriangleGeometry(
				Vec3Df(2, 2, 2),
				Vec3Df(3, 3, 3),
				Vec3Df(4, 4, 4)));

			this->triangles->get()->push_back(&TriangleGeometry(
				Vec3Df(3, 3, 3),
				Vec3Df(4, 4, 4), 
				Vec3Df(5, 5, 5)));

			this->triangles->get()->push_back(&TriangleGeometry(
				Vec3Df(4, 4, 4),
				Vec3Df(5, 5, 5), 
				Vec3Df(6, 6, 6)));

		}

		[TestCleanup()]
		void After()
		{
			delete this->xTree;
			delete this->yTree;
			delete this->zTree;
		}


		[TestMethod]
		void testBTreeEmpty()
		{
			std::vector<std::shared_ptr<IGeometry>> xTriangles = xTree->GetTriangles(-999, 999);
			std::vector<std::shared_ptr<IGeometry>> yTriangles = yTree->GetTriangles(-999, 999);
			std::vector<std::shared_ptr<IGeometry>> zTriangles = zTree->GetTriangles(-999, 999);

			Assert::AreEqual(0, xTriangles.size());
			Assert::AreEqual(0, yTriangles.size());
			Assert::AreEqual(0, zTriangles.size());

		}

		[TestMethod]
		void testBTreeSingleEntry()
		{
			/*IGeometry* test = (*this->triangles->get())[0];

			TriangleGeometry* myTest;
			myTest = (TriangleGeometry*)test;*/

			//const Vec3Df vec1 = myTest->getVertex1();

			//Vec3Df*  vec0 = &myTest->getVertex0();
			//Vec3Df vec1 = myTest->getVertex1();
			//Vec3Df vec2 = myTest->getVertex2();
		}

		[TestMethod]
		void testBTreeLeftChildAssignment()
		{
			Assert::Fail();
		}
		[TestMethod]
		void testBTreeRightChildAssignment()
		{
			Assert::Fail();
		}

		[TestMethod]
		void testBTreeBalanceLeft()
		{
			Assert::Fail();
		}
		[TestMethod]
		void testBTreeBalanceRight()
		{
			Assert::Fail();
		}

		[TestMethod]
		void testBTreeLeftLeftChildAssignment()
		{
			Assert::Fail();
		}
		[TestMethod]
		void testBTreeLeftRightChildAssignment()
		{
			Assert::Fail();
		}
		[TestMethod]
		void testBTreeRightLeftChildAssignment()
		{
			Assert::Fail();
		}
		[TestMethod]
		void testBTreeRightRightChildAssignment()
		{
			Assert::Fail();
		}

		[TestMethod]
		void testBTreeBalanceSubnodeRight()
		{
			Assert::Fail();
		}
		[TestMethod]
		void testBTreeBalanceSubnodeLeft()
		{
			Assert::Fail();
		}
	};
}
