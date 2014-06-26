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
#include "BTreeAccelerator.h"
#include "ConstantTexture.h"
#include "Constants.h"
#include "DiskGeometry.h"
#include "ILight.h"
#include "IMaterial.h"
#include "MeshGeometry.h"
#include "Octree.h"
#include "OrenNayarBRDF.h"
#include "PerspectiveCamera.h"
#include "PlaneGeometry.h"
#include "PointLight.h"
#include "RayTracer.h"
#include "Scene.h"
#include "SphereGeometry.h"
#include "TriangleGeometry.h"

#include "BlinnPhongBRDF.h"

//temporary variables
Vec3Df testRayOrigin;
Vec3Df testRayDestination;

// indicate the model and material-file that will be loaded.
// the model and material file must be affiliated
const char * model_obj = "models/bunny.obj";

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

		break;
	}



	//here I use it to get the coordinates of a ray, which I then draw in the debug function.
	produceRay(x, y, testRayOrigin, testRayDestination);

	std::cout << t << " pressed! The mouse was in location " << x << "," << y << "!" << std::endl;
}

void createScene1(Scene *scene) {
	// Determines the number of light samples taken.
	// Setting this to a low number will cause visible noise but decreases rendering time.
	// Setting this to a high number will cause a better result at the expense of rendering time
	scene->setLightSampleDensity(0.0f);

	// Ambient light color
	scene->setAmbientLight(Vec3Df(0.1f, 0.1f, 0.1f));

	// Ambient occlusion samples per surface intersection (0 = no ambient occlusion)
	scene->setAmbientOcclusionSamples(8);

	// sqrt(Number) of samples per pixel (16x16)
	scene->setSamplesPerPixel(4);
	
	// Create an emissive material
	auto lightMaterial = std::make_shared<IMaterial>();
	lightMaterial->setEmissiveness(1.0f);

	// Create a green diffuse material
	auto greenMaterial = std::make_shared<IMaterial>();
	greenMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0.2f, 0.4f, 0.2f)));

	// Create a mesh and add it to the scene
	auto mesh = std::make_shared<MeshGeometry>(&MyMesh);
	mesh->setAccelerationStructure(std::make_shared<BTreeAccelerator>());
	scene->addGeometry(mesh);

	// Create a floor plane
	auto floorPlane = std::make_shared<PlaneGeometry>(Vec3Df(0, 1, 0), 0.209548995f);
	floorPlane->setMaterial(greenMaterial);
	scene->addGeometry(floorPlane);

	// Create a point light at every light position
	for (std::vector<Vec3Df>::iterator it = MyLightPositions.begin(); it != MyLightPositions.end(); ++it) {
		// Create a disk
		Vec3Df position = (*it);
		Vec3Df target = Vec3Df(0, 0.21f, 0);
		Vec3Df normal = target - position;
		normal.normalize();

		// Create a disk at position facing to target with radius 0.125f
		auto disk = std::make_shared<DiskGeometry>(normal, position, 0.125f);
		disk->setMaterial(lightMaterial);
		scene->addGeometry(disk);

		// Create a light source from the disk
		auto diskLight = std::make_shared<AreaLight>(disk);
		diskLight->setIntensity(1.0f);
		diskLight->setFalloff(0.01f);
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
}

void createScene3(Scene *scene) {
	scene->setMaxTraceDepth(4);
	scene->setAmbientOcclusionSamples(8);
	scene->setSamplesPerPixel(16);
	scene->setPathTracingEnabled(true);
	scene->setAmbientLight(Vec3Df(1, 1, 1));

	auto grass = std::make_shared<IMaterial>();
	grass->setAmbientReflectance(1.0f);
	grass->setDiffuseReflectance(1.0f);
	grass->setSpecularReflectance(0.0f);
	grass->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0.2f, 0.4f, 0.2f)));

	auto white = std::make_shared<IMaterial>();
	white->setAmbientReflectance(1.0f);
	white->setDiffuseReflectance(1.0f);
	white->setSpecularReflectance(0.0f);
	white->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1.0f, 1.0f, 1.0f)));

	auto red = std::make_shared<IMaterial>();
	red->setAmbientReflectance(1.0f);
	red->setDiffuseReflectance(1.0f);
	red->setSpecularReflectance(0.0f);
	red->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1.0f, 0.0f, 0.0f)));

	auto blue = std::make_shared<IMaterial>();
	blue->setAmbientReflectance(1.0f);
	blue->setDiffuseReflectance(1.0f);
	blue->setSpecularReflectance(0.0f);
	blue->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0.0f, 0.0f, 1.0f)));

	auto floor = std::make_shared<PlaneGeometry>(Vec3Df(0, 1, 0), -0.5f);
	floor->setMaterial(grass);

	auto sphere1 = std::make_shared<SphereGeometry>(Vec3Df(-0.3f, -0.3f, 1.0f), 0.2f);
	sphere1->setMaterial(white);

	auto sphere2 = std::make_shared<SphereGeometry>(Vec3Df(-0.2f, -0.3f, 0.2f), 0.2f);
	sphere2->setMaterial(red);

	auto sphere3 = std::make_shared<SphereGeometry>(Vec3Df(0.2f, -0.25f, 0.55f), 0.25f);
	sphere3->setMaterial(white);

	auto sphere4 = std::make_shared<SphereGeometry>(Vec3Df(0.45f, -0.3f, 0.1f), 0.2f);
	sphere4->setMaterial(blue);

	auto sphere5 = std::make_shared<SphereGeometry>(Vec3Df(0.05f, -0.4f, 0.0f), 0.1f);
	sphere5->setMaterial(white);

	auto sphere6 = std::make_shared<SphereGeometry>(Vec3Df(0.5f, -0.45f, -0.13f), 0.075f);
	sphere6->setMaterial(white);

	auto sphere7 = std::make_shared<SphereGeometry>(Vec3Df(0.5f, -0.2f, 3.7f), 0.3f);
	sphere7->setMaterial(red);

	auto sphere8 = std::make_shared<SphereGeometry>(Vec3Df(1.5f, -0.2f, 2.7f), 0.3f);
	sphere8->setMaterial(blue);

	scene->addGeometry(floor);
	scene->addGeometry(sphere1);
	scene->addGeometry(sphere2);
	scene->addGeometry(sphere3);
	scene->addGeometry(sphere4);
	scene->addGeometry(sphere5);
	scene->addGeometry(sphere6);
	scene->addGeometry(sphere7);
	scene->addGeometry(sphere8);
}

// Because destructors...
Mesh bunnyMesh;
Mesh teapotMesh;
Mesh suzanneMesh;

void createCornellBox(Scene *scene) {
	scene->setSamplesPerPixel(4);
	scene->setMaxTraceDepth(5);
	scene->setPathTracingEnabled(true);

	bunnyMesh.loadMesh("models/bunny2.obj", true);
	bunnyMesh.computeVertexNormals();

	teapotMesh.loadMesh("models/teapot.obj", true);
	teapotMesh.computeVertexNormals();

	suzanneMesh.loadMesh("models/suzanne.obj", true);
	suzanneMesh.computeVertexNormals();

	auto bunny = std::make_shared<MeshGeometry>(&bunnyMesh);
	auto teapot = std::make_shared<MeshGeometry>(&teapotMesh);
	auto suzanne = std::make_shared<MeshGeometry>(&suzanneMesh);

	auto bunnyMaterial = std::make_shared<IMaterial>();
	bunnyMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1)));
	bunnyMaterial->setDiffuseReflectance(0.0f);
	bunnyMaterial->setSpecularReflectance(0.0f);
	bunnyMaterial->setTransparency(1.0f);
	bunnyMaterial->setAbsorbance(0.01f);
	bunnyMaterial->setRefractiveIndex(1.517f);

	auto teapotMaterial = std::make_shared<IMaterial>();
	teapotMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1)));
	teapotMaterial->setDiffuseReflectance(0.0f);
	teapotMaterial->setSpecularReflectance(1.0f);

	auto suzanneMaterial = std::make_shared<IMaterial>();
	suzanneMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1)));
	suzanneMaterial->setDiffuseReflectance(1.0f);
	suzanneMaterial->setSpecularReflectance(1.0f);
	suzanneMaterial->setShininess(80.0f);
	suzanneMaterial->setRoughness(0.3f);
	suzanneMaterial->setDiffuseBRDF<OrenNayarBRDF>();
	suzanneMaterial->setSpecularBRDF<BlinnPhongBRDF>();

	bunny->setMaterial(bunnyMaterial);
	teapot->setMaterial(teapotMaterial);
	suzanne->setMaterial(suzanneMaterial);

	scene->addGeometry(bunny);
	scene->addGeometry(teapot);
	scene->addGeometry(suzanne);

	auto darkGreen = std::make_shared<IMaterial>();
	darkGreen->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0.000000f, 0.320000f, 0.000000f)));
	darkGreen->setDiffuseReflectance(1.0f);
	darkGreen->setSpecularReflectance(0.0f);
	darkGreen->setSpecularBRDF<BlinnPhongBRDF>();

	auto halveRed = std::make_shared<IMaterial>();
	halveRed->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0.560024f, 0.000000f, 0.000000f)));
	halveRed->setDiffuseReflectance(1.0f);
	halveRed->setSpecularReflectance(0.0f);
	halveRed->setSpecularBRDF<BlinnPhongBRDF>();

	auto kahki = std::make_shared<IMaterial>();
	kahki->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0.800000f, 0.659341f, 0.439560f)));
	kahki->setDiffuseReflectance(1.0f);
	kahki->setSpecularReflectance(0.0f);
	kahki->setSpecularBRDF<BlinnPhongBRDF>();

	auto areaLightMaterial = std::make_shared<IMaterial>();
	areaLightMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(1, 1, 1)));
	areaLightMaterial->setDiffuseReflectance(0.0f);
	areaLightMaterial->setSpecularReflectance(0.0f);
	areaLightMaterial->setEmissiveness(1.0f);

	auto boxMaterial = std::make_shared<IMaterial>();
	boxMaterial->setTexture(std::make_shared<ConstantTexture>(Vec3Df(0, 0, 1)));
	boxMaterial->setDiffuseReflectance(1.0f);
	boxMaterial->setSpecularReflectance(0.0f);

	Vec3Df tallBoxVerts[8];
	Vec3Df tallBoxTranslation = Vec3Df(-3.68500f, 1.64720f, -3.51631f);
	tallBoxVerts[0] = Vec3Df(-0.54500f, -1.64917f, 1.04631f) + tallBoxTranslation;
	tallBoxVerts[1] = Vec3Df(1.03500f, -1.64956f, 0.55631f) + tallBoxTranslation;
	tallBoxVerts[2] = Vec3Df(0.54500f, -1.65083f, -1.04369f) + tallBoxTranslation;
	tallBoxVerts[3] = Vec3Df(-1.03500f, -1.65043f, -0.54369f) + tallBoxTranslation;
	tallBoxVerts[4] = Vec3Df(-0.54500f, 1.65083f, 1.04369f) + tallBoxTranslation;
	tallBoxVerts[5] = Vec3Df(1.03500f, 1.65044f, 0.55369f) + tallBoxTranslation;
	tallBoxVerts[6] = Vec3Df(0.54500f, 1.64917f, -1.04631f) + tallBoxTranslation;
	tallBoxVerts[7] = Vec3Df(-1.03500f, 1.64957f, -0.54631f) + tallBoxTranslation;
	
	Vec3Df shortBoxVerts[8];
	Vec3Df shortBoxTranslation = Vec3Df(-1.86000f, 0.82366f, -1.68566f);
	shortBoxVerts[0] = Vec3Df(-1.04000f, -0.82457f, 0.54566f) + shortBoxTranslation;
	shortBoxVerts[1] = Vec3Df(0.56000f, -0.82418f, 1.03566f) + shortBoxTranslation;
	shortBoxVerts[2] = Vec3Df(1.04000f, -0.82545f, -0.56434f) + shortBoxTranslation;
	shortBoxVerts[3] = Vec3Df(-0.54000f, -0.82582f, -1.03434f) + shortBoxTranslation;
	shortBoxVerts[4] = Vec3Df(-1.04000f, 0.82543f, 0.54434f) + shortBoxTranslation;
	shortBoxVerts[5] = Vec3Df(0.56000f, 0.82582f, 1.03434f) + shortBoxTranslation;
	shortBoxVerts[6] = Vec3Df(1.04000f, 0.82455f, -0.56566f) + shortBoxTranslation;
	shortBoxVerts[7] = Vec3Df(-0.54000f, 0.82418f, -1.03566f) + shortBoxTranslation;

	Vec3Df areaLightVerts[4];
	//Vec3Df areaLightTranslation = Vec3Df(-2.78000f, 5.48577f, -2.79937f);
	Vec3Df areaLightTranslation = Vec3Df(-2.78000f, 5.48000f, -2.79937f);
	areaLightVerts[0] = Vec3Df(-0.65000f, 0.00042f, 0.52500f) + areaLightTranslation;
	areaLightVerts[1] = Vec3Df(0.65000f, 0.00042f, 0.52500f) + areaLightTranslation;
	areaLightVerts[2] = Vec3Df(0.65000f, -0.00042f, -0.52500f) + areaLightTranslation;
	areaLightVerts[3] = Vec3Df(-0.65000f, -0.00042f, -0.52500f) + areaLightTranslation;

	Vec3Df boxVerts[8];
	Vec3Df boxBottomTranslation = Vec3Df(-2.76400f, -0.00223f, -2.79600f);
	Vec3Df boxTopTranslation = Vec3Df(-2.78000f, 5.48577f, -2.80037f);
	boxVerts[0] = Vec3Df(-2.76400f, 0.00223f, 2.79600f) + boxBottomTranslation;
	boxVerts[1] = Vec3Df(2.76400f, 0.00223f, 2.79600f) + boxBottomTranslation;
	boxVerts[2] = Vec3Df(2.76400f, -0.00223f, -2.79600f) + boxBottomTranslation;
	boxVerts[3] = Vec3Df(-2.73200f, -0.00223f, -2.79600f) + boxBottomTranslation;
	boxVerts[4] = Vec3Df(-2.78000f, 0.00223f, 2.79600f) + boxTopTranslation;
	boxVerts[5] = Vec3Df(2.78000f, 0.00223f, 2.79600f) + boxTopTranslation;
	boxVerts[6] = Vec3Df(2.78000f, -0.00223f, -2.79600f) + boxTopTranslation;
	boxVerts[7] = Vec3Df(-2.78000f, -0.00223f, -2.79600f) + boxTopTranslation;

	auto tallBoxFrontTri1 = std::make_shared<TriangleGeometry>(tallBoxVerts[4], tallBoxVerts[0], tallBoxVerts[1]);
	auto tallBoxFrontTri2 = std::make_shared<TriangleGeometry>(tallBoxVerts[4], tallBoxVerts[1], tallBoxVerts[5]);
	auto tallBoxBackTri1 = std::make_shared<TriangleGeometry>(tallBoxVerts[2], tallBoxVerts[3], tallBoxVerts[7]);
	auto tallBoxBackTri2 = std::make_shared<TriangleGeometry>(tallBoxVerts[2], tallBoxVerts[7], tallBoxVerts[6]);
	auto tallBoxLeftTri1 = std::make_shared<TriangleGeometry>(tallBoxVerts[0], tallBoxVerts[4], tallBoxVerts[3]);
	auto tallBoxLeftTri2 = std::make_shared<TriangleGeometry>(tallBoxVerts[3], tallBoxVerts[4], tallBoxVerts[7]);
	auto tallBoxRightTri1 = std::make_shared<TriangleGeometry>(tallBoxVerts[5], tallBoxVerts[1], tallBoxVerts[2]);
	auto tallBoxRightTri2 = std::make_shared<TriangleGeometry>(tallBoxVerts[5], tallBoxVerts[2], tallBoxVerts[6]);
	auto tallBoxTopTri1 = std::make_shared<TriangleGeometry>(tallBoxVerts[4], tallBoxVerts[5], tallBoxVerts[6]);
	auto tallBoxTopTri2 = std::make_shared<TriangleGeometry>(tallBoxVerts[6], tallBoxVerts[7], tallBoxVerts[4]);
	auto tallBoxBottomTri1 = std::make_shared<TriangleGeometry>(tallBoxVerts[1], tallBoxVerts[0], tallBoxVerts[2]);
	auto tallBoxBottomTri2 = std::make_shared<TriangleGeometry>(tallBoxVerts[3], tallBoxVerts[2], tallBoxVerts[0]);

	auto shortBoxFrontTri1 = std::make_shared<TriangleGeometry>(shortBoxVerts[4], shortBoxVerts[0], shortBoxVerts[1]);
	auto shortBoxFrontTri2 = std::make_shared<TriangleGeometry>(shortBoxVerts[4], shortBoxVerts[1], shortBoxVerts[5]);
	auto shortBoxBackTri1 = std::make_shared<TriangleGeometry>(shortBoxVerts[2], shortBoxVerts[3], shortBoxVerts[7]);
	auto shortBoxBackTri2 = std::make_shared<TriangleGeometry>(shortBoxVerts[2], shortBoxVerts[7], shortBoxVerts[6]);
	auto shortBoxLeftTri1 = std::make_shared<TriangleGeometry>(shortBoxVerts[0], shortBoxVerts[4], shortBoxVerts[3]);
	auto shortBoxLeftTri2 = std::make_shared<TriangleGeometry>(shortBoxVerts[3], shortBoxVerts[4], shortBoxVerts[7]);
	auto shortBoxRightTri1 = std::make_shared<TriangleGeometry>(shortBoxVerts[5], shortBoxVerts[1], shortBoxVerts[2]);
	auto shortBoxRightTri2 = std::make_shared<TriangleGeometry>(shortBoxVerts[5], shortBoxVerts[2], shortBoxVerts[6]);
	auto shortBoxTopTri1 = std::make_shared<TriangleGeometry>(shortBoxVerts[4], shortBoxVerts[5], shortBoxVerts[6]);
	auto shortBoxTopTri2 = std::make_shared<TriangleGeometry>(shortBoxVerts[6], shortBoxVerts[7], shortBoxVerts[4]);
	auto shortBoxBottomTri1 = std::make_shared<TriangleGeometry>(shortBoxVerts[1], shortBoxVerts[0], shortBoxVerts[2]);
	auto shortBoxBottomTri2 = std::make_shared<TriangleGeometry>(shortBoxVerts[3], shortBoxVerts[2], shortBoxVerts[0]);

	auto areaLightTri1 = std::make_shared<TriangleGeometry>(areaLightVerts[1], areaLightVerts[0], areaLightVerts[2]);
	auto areaLightTri2 = std::make_shared<TriangleGeometry>(areaLightVerts[3], areaLightVerts[2], areaLightVerts[0]);

	//auto boxFrontTri1 = std::make_shared<TriangleGeometry>(boxVerts[0], boxVerts[4], boxVerts[1]);
	//auto boxFrontTri2 = std::make_shared<TriangleGeometry>(boxVerts[1], boxVerts[4], boxVerts[5]);
	auto boxBackTri1 = std::make_shared<TriangleGeometry>(boxVerts[3], boxVerts[2], boxVerts[7]);
	auto boxBackTri2 = std::make_shared<TriangleGeometry>(boxVerts[7], boxVerts[2], boxVerts[6]);
	auto boxLeftTri1 = std::make_shared<TriangleGeometry>(boxVerts[4], boxVerts[0], boxVerts[3]);
	auto boxLeftTri2 = std::make_shared<TriangleGeometry>(boxVerts[4], boxVerts[3], boxVerts[7]);
	auto boxRightTri1 = std::make_shared<TriangleGeometry>(boxVerts[1], boxVerts[5], boxVerts[2]);
	auto boxRightTri2 = std::make_shared<TriangleGeometry>(boxVerts[2], boxVerts[5], boxVerts[6]);
	auto boxTopTri1 = std::make_shared<TriangleGeometry>(boxVerts[5], boxVerts[4], boxVerts[6]);
	auto boxTopTri2 = std::make_shared<TriangleGeometry>(boxVerts[7], boxVerts[6], boxVerts[4]);
	auto boxBottomTri1 = std::make_shared<TriangleGeometry>(boxVerts[0], boxVerts[1], boxVerts[2]);
	auto boxBottomTri2 = std::make_shared<TriangleGeometry>(boxVerts[2], boxVerts[3], boxVerts[0]);

	auto areaLightTri1Light = std::make_shared<AreaLight>(areaLightTri1);
	auto areaLightTri2Light = std::make_shared<AreaLight>(areaLightTri2);
	areaLightTri1Light->setIntensity(1.0f);
	areaLightTri2Light->setIntensity(1.0f);
	areaLightTri1Light->setFalloff(0.0f);
	areaLightTri2Light->setFalloff(0.0f);

	tallBoxFrontTri1->setMaterial(kahki);
	tallBoxFrontTri2->setMaterial(kahki);
	tallBoxBackTri1->setMaterial(kahki);
	tallBoxBackTri2->setMaterial(kahki);
	tallBoxLeftTri1->setMaterial(kahki);
	tallBoxLeftTri2->setMaterial(kahki);
	tallBoxRightTri1->setMaterial(kahki);
	tallBoxRightTri2->setMaterial(kahki);
	tallBoxTopTri1->setMaterial(kahki);
	tallBoxTopTri2->setMaterial(kahki);
	tallBoxBottomTri1->setMaterial(kahki);
	tallBoxBottomTri2->setMaterial(kahki);
	
	shortBoxFrontTri1->setMaterial(kahki);
	shortBoxFrontTri2->setMaterial(kahki);
	shortBoxBackTri1->setMaterial(kahki);
	shortBoxBackTri2->setMaterial(kahki);
	shortBoxLeftTri1->setMaterial(kahki);
	shortBoxLeftTri2->setMaterial(kahki);
	shortBoxRightTri1->setMaterial(kahki);
	shortBoxRightTri2->setMaterial(kahki);
	shortBoxTopTri1->setMaterial(kahki);
	shortBoxTopTri2->setMaterial(kahki);
	shortBoxBottomTri1->setMaterial(kahki);
	shortBoxBottomTri2->setMaterial(kahki);

	areaLightTri1->setMaterial(areaLightMaterial);
	areaLightTri2->setMaterial(areaLightMaterial);

	//boxFrontTri1->setMaterial(kahki);
	//boxFrontTri2->setMaterial(kahki);
	boxBackTri1->setMaterial(kahki);
	boxBackTri2->setMaterial(kahki);
	boxLeftTri1->setMaterial(halveRed);
	boxLeftTri2->setMaterial(halveRed);
	boxRightTri1->setMaterial(darkGreen);
	boxRightTri2->setMaterial(darkGreen);
	boxTopTri1->setMaterial(kahki);
	boxTopTri2->setMaterial(kahki);
	boxBottomTri1->setMaterial(kahki);
	boxBottomTri2->setMaterial(kahki);

	scene->addGeometry(tallBoxFrontTri1);
	scene->addGeometry(tallBoxFrontTri2);
	scene->addGeometry(tallBoxBackTri1);
	scene->addGeometry(tallBoxBackTri2);
	scene->addGeometry(tallBoxLeftTri1);
	scene->addGeometry(tallBoxLeftTri2);
	scene->addGeometry(tallBoxRightTri1);
	scene->addGeometry(tallBoxRightTri2);
	scene->addGeometry(tallBoxTopTri1);
	scene->addGeometry(tallBoxTopTri2);
	scene->addGeometry(tallBoxBottomTri1);
	scene->addGeometry(tallBoxBottomTri2);

	scene->addGeometry(shortBoxFrontTri1);
	scene->addGeometry(shortBoxFrontTri2);
	scene->addGeometry(shortBoxBackTri1);
	scene->addGeometry(shortBoxBackTri2);
	scene->addGeometry(shortBoxLeftTri1);
	scene->addGeometry(shortBoxLeftTri2);
	scene->addGeometry(shortBoxRightTri1);
	scene->addGeometry(shortBoxRightTri2);
	scene->addGeometry(shortBoxTopTri1);
	scene->addGeometry(shortBoxTopTri2);
	scene->addGeometry(shortBoxBottomTri1);
	scene->addGeometry(shortBoxBottomTri2);

	scene->addGeometry(areaLightTri1);
	scene->addGeometry(areaLightTri2);

	//scene->addGeometry(boxFrontTri1);
	//scene->addGeometry(boxFrontTri2);
	scene->addGeometry(boxBackTri1);
	scene->addGeometry(boxBackTri2);
	scene->addGeometry(boxLeftTri1);
	scene->addGeometry(boxLeftTri2);
	scene->addGeometry(boxRightTri1);
	scene->addGeometry(boxRightTri2);
	scene->addGeometry(boxTopTri1);
	scene->addGeometry(boxTopTri2);
	scene->addGeometry(boxBottomTri1);
	scene->addGeometry(boxBottomTri2);

	scene->addLight(areaLightTri1Light);
	scene->addLight(areaLightTri2Light);
}

#define SCENE 4
#define DEPTH_OF_FIELD 0

void rayTrace() {
	std::cout << "Raytracing" << std::endl;

	Scene scene;

#if SCENE == 1
	// Create a scene
	createScene1(&scene);

	// Create a perspective camera
	auto camera = std::make_shared<PerspectiveCamera>(MyCameraPosition, MyCameraTarget, MyCameraUp);
#elif SCENE == 2
	// Create a scene
	createScene2(&scene);

	// Create a perspective camera
	auto camera = std::make_shared<PerspectiveCamera>(Vec3Df(0, -0.1f, 1.5f), Vec3Df(), Vec3Df(0, 1, 0));

 #if DEPTH_OF_FIELD == 1
	camera->setAperatureRadius(0.025f);
	camera->setFocalDistance(1.1f);
 #endif
#elif SCENE == 3
	createScene3(&scene);

	auto camera = std::make_shared<PerspectiveCamera>(Vec3Df(-0.3, 0.3f, -1.2f), Vec3Df(0, -0.2f, 0), Vec3Df(0, 1, 0));

 #if DEPTH_OF_FIELD == 1
	camera->setAperatureRadius(0.05f);
	//camera->setFocalDistance(1.5f);
	camera->setFocalDistance(1.5f);
 #endif
#elif SCENE == 4
	createCornellBox(&scene);

	auto camera = std::make_shared<PerspectiveCamera>(Vec3Df(-2.7f, 2.7f, 8.0f), Vec3Df(-2.7f, 2.7f, 7.0f), Vec3Df(0, 1, 0));
#else
 #error No scene specified
#endif

	// Render the scene
	std::shared_ptr<Image> result = scene.render(camera, WindowSize_X, WindowSize_Y);

	result->writeImage("Render/result.ppm");
}