#include <cstdio>

#ifdef WIN32
 #include <glut/glut.h>
#else
 #include "glut/glut.h"
#endif

#include "raytracing.h"

#include "Image.h"

#include "ILight.h"
#include "PerspectiveCamera.h"
#include "PointLight.h"
#include "MeshGeometry.h"
#include "RayTracer.h"
#include "Scene.h"


//temporary variables
Vec3Df testRayOrigin;
Vec3Df testRayDestination;

// indicate the model and material-file that will be loaded.
// the model and material file must be affiliated
const char * model_obj = "models/dodgeColorTest.obj";

// use a counter to keep track of the render process
int rayIteration = 0;

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

void yourKeyboardFunc(unsigned char t, int x, int y)
{
	// do what you want with the keyboard input t.
	// x, y are the screen position
	switch (t)
	{
	case 'r':
		// reset the counter
		rayIteration = 0;
	}



	//here I use it to get the coordinates of a ray, which I then draw in the debug function.
	produceRay(x, y, testRayOrigin, testRayDestination);

	std::cout<<t<<" pressed! The mouse was in location "<<x<<","<<y<<"!"<<std::endl;
}

void rayTrace() {
	std::cout << "Raytracing" << std::endl;

	// Create a scene
	Scene scene = Scene();

	// Create a perspective camera
	auto camera = std::make_shared<PerspectiveCamera>(MyCameraPosition, MyCameraTarget, MyCameraUp);

	// Create a mesh and add it to the scene
	auto mesh = std::make_shared<MeshGeometry>(&MyMesh);

	scene.addGeometry(mesh);

	// Create a point light at every light position
	for (std::vector<Vec3Df>::iterator it = MyLightPositions.begin(); it != MyLightPositions.end(); ++it) {
		scene.addLight(std::make_shared<PointLight>((*it), Vec3Df(0.8f, 0.8f, 0.8f)));
	}

	// Render the scene
	std::shared_ptr<Image> result = scene.render(camera, WindowSize_X, WindowSize_Y);

	result->writeImage("Render/result.ppm");
}
