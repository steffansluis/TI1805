#include "stdafx.h"

#include "..\BTreeAccelerator.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Assignment4_Testing
{
	[TestClass]
	public ref class BTreeAcceleratorTest
	{
	private:

		std::shared_ptr<BTreeAccelerator> * accelerator;

		std::vector<std::shared_ptr<IGeometry>> * validGeometry;
		std::vector<std::shared_ptr<IGeometry>> * invalidGeometry;


	public: 

		[TestInitialize()]
		void Before()
		{
			// initialize a new BTreeAccelerator
			accelerator = new std::shared_ptr<BTreeAccelerator>(std::make_shared<BTreeAccelerator>());

			// Initialize geometry for the accelerator
			//
			// The BTreeAccelerator is solely meant for triangles. This is the valid geometry


			// 
			// If any other geometry is used, we should expect the accelerator to throw an exception.
		}

		[TestCleanup()]
		void After()
		{
			delete accelerator;
		}


		[TestMethod]
		void TestMethod1()
		{
		}
	};
}
