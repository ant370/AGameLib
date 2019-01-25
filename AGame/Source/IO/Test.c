// Test.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>  
#include "IO.h"
#include "Math.h" 
#include <stdbool.h> 
#include "Graphics.h"
#include "Resource.h"

int main(int argc, char *argv[])
{   
	IOState state = IOInit();
	bool running = true;


	//Load a shader
	char * v = IOLoadFile("Shaders/vert.glsl");
	char * f = IOLoadFile("Shaders/frag.glsl"); 
    GraphicsShaderProgram prog = GraphicsCreateShader(v,f);
  
	printf("%s", prog.errorFrag);
	printf("%s", prog.errorVert);

	GameResource * md2model = GameResourceLoadMD2("faerie.md2");   
	

	//Create scene
	GraphicsScene *scene = GraphicsSceneCreate();
	GraphicsObject *box = GraphicsObjectCreate();
	GraphicsMaterial mat;

	GameResource * resc = GameResourceLoadPNG("test.png"); 
	unsigned int text = GraphicsGenerateTextureFromImage(resc->data,
			 resc->info.image.width,
			 resc->info.image.height);
 
 
	GraphicsMaterial mat2;
	mat2.shader = prog.programId; 
	mat2.albido = (GraphicsTexture) {  text  }; 
	
	
	mat.albido = (GraphicsTexture) {   text }; 
	mat.shader = prog.programId; 
	box->location = (Vec4) {-5.0f,2.0f,-2.0f, 1.0f};
	GraphicsObjectSetGeom(box, md2model->info.md2.model.geom);
	GraphicsSceneAddObject(scene, box);	
	box->material = &mat2; 
	box->scale = (Vec4) { 0.5f, 0.5f, 0.5f, 0.5f};
	

	GraphicsObject *box2 = GraphicsObjectCreate();
	box2->material = &mat;
	box2->location = (Vec4) {5.0f, 1.0f,0.0f,1.0f};
	box2->scale = (Vec4) { 3.0f, 3.0f, 3.0f, 1.0f};
	GraphicsObjectSetGeom(box2, GraphicsGeomCreateBox());
	GraphicsSceneAddObject(scene, box2);

	GraphicsObject *box3 = GraphicsObjectCreate();
	box3->material = &mat;
	box3->location = (Vec4) {6.0f, 0.0f,3.0f,1.0f};
	box3->scale = (Vec4) { 1.0f,163.0f, 1.0f, 1.0f};
	GraphicsObjectSetGeom(box3, GraphicsGeomCreateBox());
	GraphicsSceneAddObject(scene, box3);

	
	GraphicsObject *box4 = GraphicsObjectCreate();
	box4->material = &mat;
	box4->location = (Vec4) {-3.0f, 0.0f,3.0f,1.0f};
	box4->scale = (Vec4) { 1.0f,1.0f, 1.0f, 1.0f};
	box4->rotation = (Vec4) { 0.0f,1.0f, 0.0f, 1.0f};
	GraphicsObjectSetGeom(box4, GraphicsGeomSphere(5));
	GraphicsSceneAddObject(scene, box4);



	GraphicsObject *plane = GraphicsObjectCreate();
	plane->material = &mat;
	plane->rotation = (Vec4) {0.0f, 1.0f, 0.0f, 1.0f};
	plane->rotationAngle = 0.20f * 3.142f / 2.0f;
	plane->scale = (Vec4) { 50.0f, 0.20f, 50.0f, 1.0f};
	plane->location = (Vec4) { 5.0f, 0.0f,  5.0f, 1.0f};
	GraphicsObjectSetGeom(plane, GraphicsGeomCreateBox());
	GraphicsSceneAddObject(scene, plane);

	scene->cameraLocation = (Vec4) {   -50.0f,  75.0f,-50.0f, 1.0f};
	scene->cameraLookat = (Vec4) {0.0f, 0.0f,0.0f,1.0f};
	scene->directionalLight.direction = (Vec4) {8.0f, 8.0f,  8.0f,1.0f};
	scene->directionalLight.color = (Vec4) { 0.10f, 0.10f, 0.10f, 1.0f};

	
	scene->directionalLight.direction = (Vec4) {8.0f, 8.0f, 8.0f,1.0f};
	scene->directionalLight.color = (Vec4) { 0.11f, 0.01f, 0.01f, 1.0f};
	scene->pointLights[0].color = (Vec4) { 0.01f, 0.101f, 0.001f, 1.0f};
	scene->pointLights[0].position = (Vec4) { 1.0f, 5.0f, 1.0f, 1.0f};
	Uint32 startclock = 0;
	Uint32 deltaclock = 0;
	Uint32 currentFPS = 0;

	startclock = SDL_GetTicks();
 	while(running)
	{ 
		running = (IOUpdate(state)).running;  
		GraphicsRenderScene(scene);

		float t = (float)(clock()) / 1000.0f;  
 
		scene->directionalLight.direction = (Vec4) {16.0f * sin(t), 4.0f,  -9.0f*cos(t),1.0f};
		box->rotationAngle = t;
		box->rotation = (Vec4) {0.0f, 0.0f, 1.0f, 1.0f};
	}
	
 	SDL_Quit();
	return 0;
}