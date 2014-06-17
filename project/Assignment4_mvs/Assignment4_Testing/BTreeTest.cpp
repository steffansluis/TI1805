

#include "stdafx.h"

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
		std::vector<IGeometry>* triangles;
		

	public:

		[TestInitialize()]
		void Before()
		{
			this->triangles = new std::vector<IGeometry>();

			// add 5 different triangles
			(*this->triangles)[0] = TriangleGeometry(
				Vec3Df(0, 0, 0),
				Vec3Df(1, 1, 1),
				Vec3Df(2, 2, 2));

			(*this->triangles)[1] = TriangleGeometry(
				Vec3Df(1, 1, 1),
				Vec3Df(2, 2, 2), 
				Vec3Df(3, 3, 3));

			(*this->triangles)[2] = TriangleGeometry(
				Vec3Df(2, 2, 2), 
				Vec3Df(3, 3, 3), 
				Vec3Df(4, 4, 4));

			(*this->triangles)[3] = TriangleGeometry(
				Vec3Df(3, 3, 3),
				Vec3Df(4, 4, 4), 
				Vec3Df(5, 5, 5));

			(*this->triangles)[4] = TriangleGeometry(
				Vec3Df(4, 4, 4),
				Vec3Df(5, 5, 5), 
				Vec3Df(6, 6, 6));

		}

		[TestCleanup()]
		void After()
		{

		}


		[TestMethod]
		void testBTreeEmpty()
		{
			Assert::Fail();
		}

		[TestMethod]
		void testBTreeSingleEntry()
		{
			Assert::Fail();
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
