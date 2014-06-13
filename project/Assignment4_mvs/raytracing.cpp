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
	const std::vector<Triangle> & triangles = retrieveTriangles(origin, dest);


	for (int i = 0; i < triangles.size(); i++)
	{
		//std::cout << "triangle: " << i << "\n";
		// get the vertices for the triangle
		Vec3Df vertex0 = MyMesh.vertices[triangles[i].v[0]].p;
		Vec3Df vertex1 = MyMesh.vertices[triangles[i].v[1]].p;
		Vec3Df vertex2 = MyMesh.vertices[triangles[i].v[2]].p;

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
				continue;
			}

			// since the ray-length is less than already stored (or has not been stored yet), we 
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
// 
void storeMeshBTree(const std::vector<Triangle> & triangles)
{
	// Yet to be implemented



}


const std::vector<Triangle> & retrieveTriangles(const Vec3Df & origin, const Vec3Df & dest)
{
	// Yet to be implemented
	// for now, just return all triangles
	return MyMesh.triangles;
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


















