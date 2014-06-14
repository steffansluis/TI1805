#ifndef RAYTRACING_Hjdslkjfadjfasljf
#define RAYTRACING_Hjdslkjfadjfasljf
#include <vector>
#include "mesh.h"
//Welcome to your assignment...
//this is an important file, raytracing.cpp is what you need to fill out

extern Mesh MyMesh; //Main mesh
extern std::vector<Vec3Df> MyLightPositions;
extern Vec3Df MyCameraPosition; //currCamera
extern unsigned int WindowSize_X;//window resolution width
extern unsigned int WindowSize_Y;//window resolution height
extern unsigned int RayTracingResolutionX;  // largeur fenetre
extern unsigned int RayTracingResolutionY;  // largeur fenetre

class RayIntersection;
class BTree;
class BTreeNode;


//use this function for any preprocessing of the mesh.
void init();

//you can use this function to transform a click to an origin and destination
//the last two values will be changed. There is no need to define this function.
//it is defined elsewhere
void produceRay(int x_I, int y_I, Vec3Df & origin, Vec3Df & dest);


//your main function to rewrite
Vec3Df performRayTracing(const Vec3Df & origin, const Vec3Df & dest);

Vec3Df performRayTracingIteration(const Vec3Df & origin, const Vec3Df & dest, const int iteration);

// graphics techniques
Vec3Df calculateReflectionLighting(const RayIntersection & intersection, const int iteration);


// ray-tracing intersection calculations
RayIntersection* calculateIntersection(const Vec3Df & origin, const Vec3Df & dest);
Vec3Df calculateIntersection_n(Vec3Df v0, Vec3Df v1, Vec3Df v2);
bool calculateRayInsideTriangle(Vec3Df intersection, Vec3Df normal, Vec3Df v0, Vec3Df v1, Vec3Df v2);

// data-structures for the triangles
void storeMeshBTree(const std::vector<Triangle> & triangles);
const std::vector<Triangle*> * retrieveTriangles(const Vec3Df & origin, const Vec3Df & dest);



//a function to debug --- you can draw in OpenGL here
void yourDebugDraw();

//want keyboard interaction? Here it is...
void yourKeyboardFunc(char t, int x, int y);



// create a class for the ray-intersection data
class RayIntersection
{
public:

	// the length and actual vector of the ray.
	// this vector intersects with the triangle.
	// The 'ray' is the vector of the object that is intersected with.
	float rayLength;
	Vec3Df ray;

	// This is the normalized normal vector that stands 
	// orthogonal on triangle space. 
	Vec3Df normal;

	// This are the vertex-points of the triangle. 
	// These can be determined as well by requesting the 
	// triangle with the proper index.
	Vec3Df v0;
	Vec3Df v1;
	Vec3Df v2;

	// this is the original data for the ray. 
	// The equation for the ray is as follows:
	// R(t) = origin + t * destination;
	// this equation is the exact same as the 'ray' vector
	Vec3Df origin;
	Vec3Df destination;
	float t;

	// the index of the triangle that the ray intersects with.
	int index;

};



class BTree
{
public:
	enum class Coordinate { X, Y, Z };

private:

	BTreeNode* head;

	Coordinate _coordinate;

public:

	BTree(Coordinate coordinate);

	void AddNode(Triangle* data);

	std::vector<Triangle*> * GetTriangles(float lowerLimit, float upperLimit);


};


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

	

	BTreeNode(Triangle* tData, BTree::Coordinate coordinate);

	void AddNode(Triangle* data);

	int Compare(Triangle* data);
	int withinLimit(float lowerLimit, float upperLimit);

	void GetTriangles(std::vector<Triangle*> * collection, float lowerLimit, float upperLimit);

	

	void Balance();

};









#endif