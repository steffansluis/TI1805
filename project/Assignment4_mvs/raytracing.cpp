#include <cstdio>

#ifdef WIN32
#include "glut/glut.h"
#else
#include "glut/glut.h"
#include <GL/glut.h>
#endif

#include "raytracing.h"

#include "Image.h"

#include "AreaLight.h"
#include "ConstantTexture.h"
#include "DiskGeometry.h"
#include "ILight.h"
#include "IMaterial.h"
#include "PerspectiveCamera.h"
#include "PlaneGeometry.h"
#include "PointLight.h"
#include "MeshGeometry.h"
#include "RayTracer.h"
#include "Scene.h"
#include "SphereGeometry.h"

#include "BlinnPhongBRDF.h"

//temporary variables
Vec3Df testRayOrigin;
Vec3Df testRayDestination;

// indicate the model and material-file that will be loaded.
// the model and material file must be affiliated
const char * model_obj = "models/dodge.obj";

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
	glColor3f(0, 1, 1);
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

	std::cout << t << " pressed! The mouse was in location " << x << "," << y << "!" << std::endl;
}

void createScene1(Scene *scene) {
	// Determines the number of light samples taken.
	// Setting this to a low number will cause visible noise but decreases rendering time.
	// Setting this to a high number will cause a better result at the expense of rendering time
	scene->setLightSampleDensity(1000.0f);

	// Create an emissive material
	auto lightMaterial = std::make_shared<IMaterial>();
	lightMaterial->setEmissiveness(1.0f);

	// Create a red diffuse material
	auto redMaterial = std::make_shared<IMaterial>();
	redMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 0, 0)));

	// Create a mesh and add it to the scene
	auto mesh = std::make_shared<MeshGeometry>(&MyMesh);
	//mesh->setAccelerationStructure(std::make_shared<BTreeAccelerator>());
	scene->addGeometry(mesh);

	// Create a with an emissive material and add it to the scene
	auto sphere = std::make_shared<SphereGeometry>(Vec3Df(0.5f, 1, 0.5f), 0.125f);
	sphere->setMaterial(lightMaterial);
	//scene.addGeometry(sphere);
	//scene.addLight(std::make_shared<PointLight>(Vec3Df(0.5f, 1, 0.5f), Vec3Df(0.8f, 0.8f, 0.8f)));

	// Create a light source from the sphere and add it to the scene
	auto sphereLight = std::make_shared<AreaLight>(sphere);
	sphereLight->setIntensity(10.0f);
	//scene.addLight(sphereLight);

	// Create a back plane
	auto backPlane = std::make_shared<PlaneGeometry>(Vec3Df(0, 0, 1), -1);
	//scene.addGeometry(backPlane);

	// Create a floor plane
	auto floorPlane = std::make_shared<PlaneGeometry>(Vec3Df(0, 1, 0), 0.209548995f);
	scene->addGeometry(floorPlane);

	// Create a point light at every light position
	for (std::vector<Vec3Df>::iterator it = MyLightPositions.begin(); it != MyLightPositions.end(); ++it) {
		//scene.addLight(std::make_shared<PointLight>((*it), Vec3Df(0.8f, 0.8f, 0.8f)));
		// Create a disk
		Vec3Df position = (*it);
		Vec3Df normal = Vec3Df(0, 0.21f, 0) - position;
		normal.normalize();

		auto disk = std::make_shared<DiskGeometry>(normal, position, 0.125f);
		disk->setMaterial(lightMaterial);
		scene->addGeometry(disk);

		auto diskLight = std::make_shared<AreaLight>(disk);
		diskLight->setIntensity(0.035f);
		scene->addLight(diskLight);
	}
}

void createScene2(Scene *scene) {
	// Light samples taken per square unit of surface area, actual value is clamped to 1.
	scene->setLightSampleDensity(0.0f);

	// Number of samples taken per pixel, or rather the square root thereof. 4x4 samples per pixel.
	scene->setSamplesPerPixel(4);

	// Disable ambient occlusion
	scene->setAmbientOcclusionSamples(0);

	// Enable ray tracing
	scene->setPathTracingEnabled(true);

	// Set the maximum ray tracing depth to 4
	scene->setMaxTraceDepth(4);

	// Create some materials
	auto white = std::make_shared<IMaterial>();
	white->setDiffuseReflectance(1.0f);
	white->setSpecularReflectance(0.0f);

	auto red = std::make_shared<IMaterial>();
	red->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 0, 0)));
	red->setDiffuseReflectance(1.0f);
	red->setSpecularReflectance(0.0f);

	auto yellow = std::make_shared<IMaterial>();
	yellow->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 1, 0)));
	yellow->setDiffuseReflectance(1.0f);
	yellow->setSpecularReflectance(1.0f);
	yellow->setSpecularBRDF<BlinnPhongBRDF>();

	auto blue = std::make_shared<IMaterial>();
	blue->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0, 0, 1)));
	blue->setDiffuseReflectance(1.0f);
	blue->setSpecularReflectance(0.0f);

	auto mirror = std::make_shared<IMaterial>();
	mirror->setDiffuseReflectance(0.2f);
	mirror->setSpecularReflectance(0.8f);

	auto light = std::make_shared<IMaterial>();
	light->setDiffuseReflectance(0.0f);
	light->setSpecularReflectance(0.0f);
	light->setEmissiveness(1.0f);

	// Create the light source
	auto disk = std::make_shared<DiskGeometry>(Vec3Df(0, -1, 0), Vec3Df(0, 0.499f, 0), 0.2f);
	disk->setMaterial(light);

	auto diskLight = std::make_shared<AreaLight>(disk);
	diskLight->setIntensity(1.2f);

	// Create the objects
	auto floor = std::make_shared<PlaneGeometry>(Vec3Df(0, 1, 0), -0.5f);
	floor->setMaterial(white);

	auto ceiling = std::make_shared<PlaneGeometry>(Vec3Df(0, -1, 0), -0.5f);
	ceiling->setMaterial(white);

	auto back = std::make_shared<PlaneGeometry>(Vec3Df(0, 0, 1), -0.5f);
	back->setMaterial(white);

	auto left = std::make_shared<PlaneGeometry>(Vec3Df(1, 0, 0), -0.5f);
	left->setMaterial(red);

	auto right = std::make_shared<PlaneGeometry>(Vec3Df(-1, 0, 0), -0.5f);
	right->setMaterial(blue);

	auto yellowSphere = std::make_shared<SphereGeometry>(Vec3Df(-0.2f, -0.3f, 0.2f), 0.2f);
	yellowSphere->setMaterial(yellow);

	auto whiteSphere = std::make_shared<SphereGeometry>(Vec3Df(0.3f, -0.35f, 0.1f), 0.15f);
	whiteSphere->setMaterial(mirror);
	
	// Add all the objects to the scene
	scene->addGeometry(disk);
	scene->addGeometry(floor);
	scene->addGeometry(ceiling);
	scene->addGeometry(back);
	scene->addGeometry(left);
	scene->addGeometry(right);
	scene->addGeometry(yellowSphere);
	scene->addGeometry(whiteSphere);

	scene->addLight(diskLight);
	
	//scene->setAmbientLight(Vec3Df(0.5f, 0.5f, 0.5f));
}

void rayTrace() {
	std::cout << "Raytracing" << std::endl;

	// Create a scene
	Scene scene;
	createScene2(&scene);
	
	// Create a perspective camera
	//auto camera = std::make_shared<PerspectiveCamera>(MyCameraPosition, MyCameraTarget, MyCameraUp);
	auto camera = std::make_shared<PerspectiveCamera>(Vec3Df(0, -0.1f, 1.5f), Vec3Df(), Vec3Df(0, 1, 0));

	// Render the scene
	std::shared_ptr<Image> result = scene.render(camera, WindowSize_X, WindowSize_Y);

	result->writeImage("Render/result.ppm");
}