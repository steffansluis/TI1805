#include <stdio.h>


#ifdef WIN32
#include <windows.h>
#endif

//#include <GL/glut.h>
#include "glut\glut.h"

#include "raytracing.h"


//temporary variables
Vec3Df testRayOrigin;
Vec3Df testRayDestination;

// class variables

// B-tree collection objects
BTree * xTree = NULL;
BTree * yTree = NULL;
BTree * zTree = NULL;

// indicate the model and material-file that will be loaded.
// the model and material file must be affiliated
const char * model_obj = "models/dodgeColorTest.obj";

// Indicate the maximum amount of iterations that are permitted.
// This prevent the algorithm from looping back and forth between objects infinitely.
const int maxIterations = 16;

// Graphics constants
const bool ENABLE_CULLING = true;


//use this function for any preprocessing of the mesh.
void init()
{
	//load the mesh file
	//feel free to replace cube by a path to another model
	//please realize that not all OBJ files will successfully load.
	//Nonetheless, if they come from Blender, they should.
    MyMesh.loadMesh(model_obj, true);
	MyMesh.computeVertexNormals();

	//one first move: initialize the first light source
	//at least ONE light source has to be in the scene!!!
	//here, we set it to the current location of the camera
	MyLightPositions.push_back(MyCameraPosition);
}

int myIteration;

//return the color of your pixel.
Vec3Df performRayTracing(const Vec3Df & origin, const Vec3Df & dest)
{
	std::cout << "iteration " << myIteration << "\n";
	myIteration++;

	// First, load the mesh triangles into a smart storage system.
	storeMeshBTree(MyMesh.triangles);

	// call the first iteration of the ray-tracing
	return performRayTracingIteration(origin, dest, 0);
}


// @Author: Bas Boellaard
// This performs ray-tracing and has an iteration for limited amount of iterations. 
// If you need your method to do a color-lookup for another ray, please call this method with an 
// incremented iteration-count.
Vec3Df performRayTracingIteration(const Vec3Df & origin, const Vec3Df & dest, const int iteration)
{
	// if the maximum amount of iterations has been reached, return the zero-vector (black).
	if (iteration >= maxIterations)
	{
		return Vec3Df(0, 0, 0);
	}

	// calculate the intersection that will occur with these parameters
	RayIntersection* intersection = calculateIntersection(origin, dest);

	// if the intersection was non-existant, return black
	if (intersection == NULL)
	{
		return Vec3Df(0, 0, 0);
	}

	// execute all the different graphics techniques.
	Vec3Df lighting = Vec3Df(0, 0, 0);

	// reflection lighting
	lighting += calculateReflectionLighting(*intersection, iteration);

	return lighting;
}


Vec3Df calculateReflectionLighting(const RayIntersection & intersection, const int iteration)
{
	

	return Vec3Df(intersection.rayLength, intersection.rayLength, intersection.rayLength);
}








// @Author Bas Boellaard
// This method calculates the intersection between the ray originating from 'origin' and having
// 'dest' as its direction.
//
// Logic herein relies heavily on the slides from college as well as data from the following source:
// https://courses.cs.washington.edu/courses/cse457/09sp/lectures/triangle_intersection.pdf
RayIntersection * calculateIntersection(const Vec3Df & origin, const Vec3Df & dest)
{
	// if the dot-product of the origin and destination are 0, then the intersection point of the ray lies 
	// infinitely far away. If this happens, do not check for this ray.
	float dot_origin_dest = Vec3Df::dotProduct(origin, dest);

	if (abs(dot_origin_dest) < 1E-6)
	{
		return NULL;
	}

	// initialize the intersection data.
	RayIntersection*  intersection = NULL;


	// The amount of triangles that are checked should be limited. 
	const std::vector<Triangle*> * triangles = retrieveTriangles(origin, dest);


	for (int i = 0; i < triangles->size(); i++)
	{
		// get the vertices for the triangle
		Vec3Df vertex0 = MyMesh.vertices[(*triangles)[i]->v[0]].p;
		Vec3Df vertex1 = MyMesh.vertices[(*triangles)[i]->v[1]].p;
		Vec3Df vertex2 = MyMesh.vertices[(*triangles)[i]->v[2]].p;

		// calculate the normalized 'normal' component of the vertex
		Vec3Df normal = calculateIntersection_n(vertex0, vertex1, vertex2);

		// to calculate the length of the ray that goes towards the triangle, we can calculate 
		// D = n * v_0
		// we can take either v_0, v_1 or v_2; since n is orthogonal it should not matter.
		float rayLength = Vec3Df::dotProduct(normal, vertex0);

		// if this raylength is negative, then this is the wrong side of the vertex (i.e. this side is supposed to be 
		// the 'inside' of something. It can either be ignored (culling) or negated.
		if (rayLength < 0)
		{
			if (ENABLE_CULLING)
			{
				// we disregard this 
				continue;
			}
			else
			{
				rayLength *= -1;
			}
		}

		// The ray can be calculated as follows:
		// R(t) = origin + t * direction
		// the origin is `origin` and direction is `dest`. The length of the vector is now known as well. 
		//
		// The 't' component can be calculated as follows:
		// t = ( D - dot(origin, normal) ) / ( dot(direction, origin) )
		float dot_origin_normal = Vec3Df::dotProduct(origin, normal);

		float t = (rayLength - dot_origin_normal) / dot_origin_dest;

		// we now know enough to calculate the entire ray.
		// R(t) = origin + t * direction
		Vec3Df ray = origin + t * dest;
		// although called 'ray', this is actually the vector of the intersection-point between 
		// the plane of the triangle and the ray. 

		// finally, we need to check if this ray is actually inside of the triangle. 
		if (calculateRayInsideTriangle(ray, normal, vertex0, vertex1, vertex2))
		{
			// this is a valid ray. 
			// if this is the first valid ray, create the RayIntersection object
			if (intersection == NULL)
			{
				intersection = &RayIntersection();
			}
			else if (intersection->rayLength < rayLength)
			{
				// ONLY store the data is the ray-length is LESS than another object stored here.
				// this means that the new triangle is in front of the other triangle (for this pixel)
				continue;
			}

			// since the ray-length is less than the already stored data (or has not been stored yet), we 
			// can put the data for the intersection into the object.
			intersection->ray = ray;
			intersection->rayLength = rayLength;
			intersection->normal = normal;
			intersection->index = i;

			intersection->v0 = vertex0;
			intersection->v1 = vertex1;
			intersection->v2 = vertex2;

			// also store the original data here.
			intersection->origin = origin;
			intersection->destination = dest;
			intersection->t = t;
		}
	}

	// return the known intersection data
	return intersection;
}


// @Author: Bas Boellaard
// this method calculated the normalized normal-vector for the triangle.
// It takes the three vertices that make up the triangle as arguments.
Vec3Df calculateIntersection_n(Vec3Df v0, Vec3Df v1, Vec3Df v2)
{
	// The normal-vector of the triangle is the vector that is orthogonal to the plane on which
	// all three vectors lie. 
	// this can be calculated as follows:
	// n = (v0 - v2) X (v1 - v2)
	// please take not that the 'X' is NOT the dot-product, but the cross-product.
	// to normalize this, it must be divided by its own length.

	Vec3Df normalVector = Vec3Df::crossProduct((v0 - v2), (v1 - v2));

	// normalize the normal vector
	normalVector.normalize();

	return normalVector;
}


// @Author: Bas Boellaard
// This method takes 4 arguments. 
// intersection: this is the coordinate of the ray R(t) = origin + t * direction 
//    where the ray intersects with the coordinate plane of the triangle
// normal: this is the normal of the material. This is the vector that stands orthogonal 
//    to the plane of the triangle.
// v0, v1, v2: these are the coordinates of the triangle.
bool calculateRayInsideTriangle(Vec3Df intersection, Vec3Df normal, Vec3Df v0, Vec3Df v1, Vec3Df v2)
{
	// There are three conditions that need to be true for the intersection to be within the three vertices.
	// (v0 - v2) X (intersect - v2) * normal >= 0
	// (v1 - v0) X (intersect - v0) * normal >= 0
	// (v2 - v1) X (intersect - v1) * normal >= 0
	// in all three cases, a different 'edge' is calculated and checked. Q must be within each edge. Only then 
	// is the intersection actually inside of the triangle. 
	Vec3Df v0_v2 = v0 - v2;
	Vec3Df v1_v0 = v1 - v0;
	Vec3Df v2_v1 = v2 - v1;

	Vec3Df q_v2 = intersection - v2;
	Vec3Df q_v0 = intersection - v0;
	Vec3Df q_v1 = intersection - v1;

	Vec3Df cross_2 = Vec3Df::crossProduct(v0_v2, q_v2);
	Vec3Df cross_0 = Vec3Df::crossProduct(v1_v0, q_v0);
	Vec3Df cross_1 = Vec3Df::crossProduct(v2_v1, q_v1);

	if (Vec3Df::dotProduct(cross_2, normal) < 0)
	{
		return false;
	}

	if (Vec3Df::dotProduct(cross_0, normal) < 0)
	{
		return false;
	}

	if (Vec3Df::dotProduct(cross_1, normal) < 0)
	{
		return false;
	}

	// since it has passed all checks, the intersection lies within the triangle
	return true;
}




// @Author Bas Boellaard
// Store the data for the x-coordinates, y-coordinates and z-coordinates in binary trees. 
void storeMeshBTree(const std::vector<Triangle> & triangles)
{
	// Since this is an intensive task, this only needs to be done once.
	if (xTree == NULL || yTree == NULL || zTree == NULL)
	{
		// create a new B-Tree for the different coordinates
		xTree = &BTree(BTree::Coordinate::X);
		yTree = &BTree(BTree::Coordinate::Y);
		zTree = &BTree(BTree::Coordinate::Z);
		// This automatically takes care of the sorting

		for (int i = 0; i < MyMesh.triangles.size(); i++)
		{
			// add all the triangles
			xTree->AddNode(&MyMesh.triangles[i]);
			yTree->AddNode(&MyMesh.triangles[i]);
			zTree->AddNode(&MyMesh.triangles[i]);
		}
	}
}


const std::vector<Triangle*> * retrieveTriangles(const Vec3Df & origin, const Vec3Df & dest)
{
	std::vector<Triangle*> * myTriangles = NULL;

	// Figure out which coordinate {X, Y, Z} to use
	// THIS IS A DUMMY VALUE
	BTree::Coordinate coordinate = BTree::Coordinate::X;

	// figure out the bounds for the triangles.
	// THESE ARE DUMMY VALUES
	float lowerLimit = -1.0;
	float upperLimit = 1.0;

	switch (coordinate)
	{
	case BTree::Coordinate::X:
		myTriangles = xTree->GetTriangles(lowerLimit, upperLimit);
		break;
	case BTree::Coordinate::Y:
		myTriangles = yTree->GetTriangles(lowerLimit, upperLimit);
		break;
	case BTree::Coordinate::Z:
		myTriangles = zTree->GetTriangles(lowerLimit, upperLimit);
		break;

	default:
		myTriangles = &std::vector<Triangle*>();
	}

	return myTriangles;
}
























void yourDebugDraw()
{
	//draw open gl debug stuff
	//this function is called every frame

	//as an example: 
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(0,1,1);
	glBegin(GL_LINES);
	glVertex3f(testRayOrigin[0], testRayOrigin[1], testRayOrigin[2]);
	glVertex3f(testRayDestination[0], testRayDestination[1], testRayDestination[2]);
	glEnd();
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3fv(MyLightPositions[0].pointer());
	glEnd();
	glPopAttrib();

}

void yourKeyboardFunc(char t, int x, int y)
{
	// do what you want with the keyboard input t.
	// x, y are the screen position

	//here I use it to get the coordinates of a ray, which I then draw in the debug function.
	produceRay(x, y, testRayOrigin, testRayDestination);

	std::cout<<t<<" pressed! The mouse was in location "<<x<<","<<y<<"!"<<std::endl;
}














/* --------------------------- B-tree implementation -----------------------------*/

/////////////////////////// BTree /////////////////////////////////


// @Author: Bas Boellaard
// Create a new B-Tree that sorts on the indicated coordinate.
BTree::BTree(Coordinate coordinate)
{
	this->_coordinate = coordinate;
}


// @Author: Bas Boellaard
// Add a new triangle to the B-Tree. If this is the first item, it will replace the head.
void BTree::AddNode(Triangle* data)
{
	if (BTree::head == NULL)
	{
		BTree::head = &BTreeNode(data, this->_coordinate);
	}
	else
	{
		BTree::head->AddNode(data);
	}
}

// @Author: Bas Boellaard
// Returns the triangles that are within bounds of the arguments. 
// These bounds are applied to the coordinate that this B-Tree sorts on. 
std::vector<Triangle*> * BTree::GetTriangles(float lowerLimit, float upperLimit)
{
	std::vector<Triangle*> * output = &std::vector<Triangle*>();

	if (this->head != NULL)
	{
		this->head->GetTriangles(output, lowerLimit, upperLimit);
	}

	return output;
}



/////////////////////////// BTreeNode ///////////////////////////////

// @Author: Bas Boellaard
// get the average value for the indicated triangle with the current coordination
float BTreeNode::GetAverageTriangleValue(Triangle* triangle)
{
	int coordIndex;

	switch (this->_coordinate)
	{
	case BTree::Coordinate::X:
		coordIndex = 0;
		break;
	case BTree::Coordinate::Y:
		coordIndex = 1;
		break;
	case BTree::Coordinate::Z:
		coordIndex = 2;
		break;

	default: 
		return 0;
	}

	float value = MyMesh.vertices[triangle->v[0]].p[coordIndex];
	value += MyMesh.vertices[triangle->v[1]].p[coordIndex];
	value += MyMesh.vertices[triangle->v[2]].p[coordIndex];

	return value / 3.0;
}


// @Author: Bas Boellaard
// Create a new Node for the BTree. 
BTreeNode::BTreeNode(Triangle* tData, BTree::Coordinate coordinate)
{
	// set depth to '0' (no children)
	BTreeNode::depth = 0;
	this->_coordinate = coordinate;
}


// @Author: Bas Boellaard
// Add a new node to the B-Tree. The triangle will either be stored as a new node as one 
// of the childs of this node, or it will be passed on to his childs. 
void BTreeNode::AddNode(Triangle* data)
{
	// first, compare the new triangle with our current triangle.
	int comparison = Compare(data);

	// if it is smaller (or equal):
	if (comparison <= 0)
	{
		// if the left child exists, recursively make him add
		if (this->leftChild != NULL)
		{
			this->leftChild->AddNode(data);
		}
		else
		{
			// since the left child does not exist, add this as the left child. 
			this->leftChild = &BTreeNode(data, this->_coordinate);
			
			// Balance ourselves out, if necessary
			this->Balance();
		}
	}
	else
	{
		// if the right child exists, recursively make him add
		if (this->rightChild != NULL)
		{
			this->rightChild->AddNode(data);
		}
		else
		{
			// since the right child does not exist, add this as the right child
			this->rightChild = &BTreeNode(data, this->_coordinate);

			// balance ourselves out, if necessary
			this->Balance();
		}
	}
}

// @Author: Bas Boellaard
// Compares a triangle with the triangle of the current node
// Outputs -1 if the other triangle is smaller, 0 if they are equal and 1 if it is larger.
int BTreeNode::Compare(Triangle* data)
{
	// since the comparison is represented with 3 values, we take the average of those.
	float myValue = GetAverageTriangleValue(this->data);
	float hisValue = GetAverageTriangleValue(data);

	if (hisValue < myValue)
	{
		return -1;
	}
	else if (hisValue > myValue)
	{
		return 1;
	}

	return 0;	
}


// @Author: Bas Boellaard
// This method checks if the current triangle is within the set limits for the set coordinate.
// there are 3 possible outcomes:
// result = 1   : the upperLimit is smaller than the smallest proper coordinate of the triangle. (coordinates too large)
// result = 0   : the triangle is within the limits.
// result = -1  : the lowerLimit is larger than the largest proper coordinate of the triangle. (coordinates too small)
int BTreeNode::withinLimit(float lowerLimit, float upperLimit)
{
	int coordIndex;

	switch (this->_coordinate)
	{
	case BTree::Coordinate::X:
		coordIndex = 0;
		break;
	case BTree::Coordinate::Y:
		coordIndex = 1;
		break;
	case BTree::Coordinate::Z:
		coordIndex = 2;
		break;

	default:
		return 0;
	}

	// get the proper coordinates
	float coord1 = MyMesh.vertices[this->data->v[0]].p[coordIndex];
	float coord2 = MyMesh.vertices[this->data->v[1]].p[coordIndex];
	float coord3 = MyMesh.vertices[this->data->v[2]].p[coordIndex];

	// check if they are less than than the lowerLimit
	if (coord1 < lowerLimit && coord2 < lowerLimit && coord3 < lowerLimit)
	{
		// this triangle is smaller than the lower limit.
		return -1;
	}

	// check if they are larger than the upperLimit
	if (coord1 > upperLimit && coord2 > upperLimit && coord3 > upperLimit)
	{
		// this triangle is larger than the upper limit.
		return 1;
	}

	// since neither was the case, this triangle is within the hit range
	return 0;
}



// @Author: Bas Boellaard
// Retrieve the triangles that are within the bounds of the limits. 
// If the current coordinates are too small (or within bounds), the right child (with larger coordinates) of the node is called.
// If the current coordinates are too large (or within bounds), the left child (with smaller coordinates) of the node is called.
// If the triangle of the current node is within bounds, it gets added to the collection. 
void BTreeNode::GetTriangles(std::vector<Triangle*> * collection, float lowerLimit, float upperLimit)
{
	// check if the current triangle is within limits
	int withinLimits = withinLimit(lowerLimit, upperLimit);

	// if the limit is more than (or equal to) 0, we can check the left childs.
	// this means that the triangles we need have a coordinate smaller than this current coordinate.
	if (withinLimits >= 0 && this->leftChild != NULL)
	{
		this->leftChild->GetTriangles(collection, lowerLimit, upperLimit);
	}

	// if the limit is les than (or equal to) 0, we can check the right childs.
	// this means that the triangles we need have a coordinate larger than this current coordinate.
	if (withinLimits <= 0 && this->rightChild != NULL)
	{
		this->rightChild->GetTriangles(collection, lowerLimit, upperLimit);
	}

	// if however withinLimits equals 0, that means the current triangle was within bounds
	if (withinLimits == 0)
	{
		collection->push_back(this->data);
	}
}


// @Author: Bas Boellaard
// balance out the node if necessary. 
// This method will also set the proper depth per node. 
// If not, call the parent node to balance.
void BTreeNode::Balance()
{
	int leftDepth = -1;
	int rightDepth = -1;

	if (this->leftChild != NULL)
	{
		leftDepth = BTreeNode::leftChild->depth;
	}

	if (this->rightChild != NULL)
	{
		rightDepth = BTreeNode::rightChild->depth;
	}

	// first, set our own depth properly
	this->depth = max(leftDepth + 1, rightDepth + 1);


	// if the difference in depth is more than 3, it requires balancing.
	if (rightDepth - leftDepth >= 3)
	{
		// The right part of the node is too large.
		// assume L = left child ; T = current node (top) ; R = right child
		// The following changes must be made:
		// - the parent of T becomes the parent of R
		// - the child of the parent of T changes appropriately
		// - R becomes the parent of T
		// - The right child of T becomes the left child of R
		// - the left child of R becomes T
		// - the depth of T becomes the depth of L + 1
		//
		// store R in advance
		BTreeNode* R = this->rightChild;

		R->parent = this->parent;

		if (R->parent->leftChild == this)
		{
			R->parent->leftChild = R;
		}
		else if (R->parent->rightChild == this)
		{
			R->parent->rightChild = R;
		}
		else
		{
			std::cout << "something went seriously wrong!!! BTreeNode::Balance()";
		}

		this->parent = R;
		this->rightChild = R->leftChild;
		R->leftChild = this;
		this->depth = this->leftChild->depth + 1;
	}
	else if (leftDepth - rightDepth >= 3)
	{
		// The left part of the node is too large.
		// assume L = left child ; T = current node (top) ; R = right child
		// The following changes must be made:
		// - the parent of T becomes the parent of L
		// - the child of the parents of T changes appropriately
		// - L becomes the parent of T
		// - the left child of T becomes the right child of L
		// - the right child of L becomes T
		// the depth of T becomes the depth of R + 1
		//
		// store L in advance
		BTreeNode* L = this->leftChild;

		L->parent = this->parent;

		if (L->parent->leftChild == this)
		{
			L->parent->leftChild = L;
		}
		else if (L->parent->rightChild == this)
		{
			L->parent->rightChild = L;
		}
		else
		{
			std::cout << "something went seriously wrong!!! BTreeNode::Balance()";
		}

		this->parent = L;
		this->leftChild = L->rightChild;
		L->rightChild = this;
		this->depth = this->rightChild->depth + 1;
	}

	// if the tree was balanced, there is no need to set any other depths. 
	// This will be done with the recursive Balance() call to the parent nodes.
	//
	// if your parent is not null, balance him out.
	if (this->parent != NULL)
	{
		this->parent->Balance();
	}
}















