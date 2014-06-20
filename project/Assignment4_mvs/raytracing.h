#ifndef RAYTRACING_Hjdslkjfadjfasljf
#define RAYTRACING_Hjdslkjfadjfasljf

#include <vector>

#include "mesh.h"

//Welcome to your assignment...
//this is an important file, raytracing.cpp is what you need to fill out

extern Mesh MyMesh; //Main mesh
extern std::vector<Vec3Df> MyLightPositions; // A vector of point-light positions
extern Vec3Df MyCameraPosition;	// The position of the camera
extern Vec3Df MyCameraTarget;	// The position that the camera is looking at
extern Vec3Df MyCameraUp;		// The up vector of the camera
extern unsigned int WindowSize_X;//window resolution width
extern unsigned int WindowSize_Y;//window resolution height
extern unsigned int RayTracingResolutionX;  // largeur fenetre
extern unsigned int RayTracingResolutionY;  // largeur fenetre

//use this function for any preprocessing of the mesh.
void init();

//you can use this function to transform a click to an origin and destination
//the last two values will be changed. There is no need to define this function.
//it is defined elsewhere
void produceRay(int x_I, int y_I, Vec3Df & origin, Vec3Df & dest);

//a function to debug --- you can draw in OpenGL here
void yourDebugDraw();

//want keyboard interaction? Here it is...
void yourKeyboardFunc(unsigned char t, int x, int y);

// Builds and renders a scene
void rayTrace();

#endif