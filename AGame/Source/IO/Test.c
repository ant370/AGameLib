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
#include "AL/al.h"
#include "AL/alc.h"
#include "AntAudio.h"
#include "q3_c.h"



int main(int argc, char *argv[])
{   
	bool running = true; 
	struct IOState state = IOInit();

	GameResourceInit(); 
	GameResource * music = GameResourceAvailable("Example.ogg");
	
	//Load a shader
	char * v = IOLoadFile("C:/Users/antmm/LibJam/AGame/AGame/Shaders/vert.glsl");
	char * f = IOLoadFile("C:/Users/antmm/LibJam/AGame/AGame/Shaders/frag.glsl"); 
    GraphicsShaderProgram prog = GraphicsCreateShader(v,f);
   
	GameResource * md2model = GameResourceAvailable("explodes.md2");
	GameResource * fontRes = GameResourceAvailable("OpenSans-Bold.ttf");

	unsigned int fontTexture = GraphicsGenerateTextureFromFont(fontRes->info.font.bitmap, 512, 512);
	
	GameResourceLogResources(); 
	//Create scene
	GraphicsScene *scene = GraphicsSceneCreate();
	GraphicsObject *box = GraphicsObjectCreate();
	GraphicsMaterial mat;

	GameResource * resc = GameResourceAvailable("1200px-Mandelbrot_set_image.png"); 
	unsigned int text = GraphicsGenerateTextureFromImage(resc->data,
			 resc->info.image.width,
			 resc->info.image.height);
 
 
	unsigned int checker = GraphicsGenerateCheckerTexture(64);
	
	GraphicsMaterial mat2;
	mat2.shader = prog.programId; 
	mat2.albido = (GraphicsTexture) {  text  }; 
	
	
	mat.albido = (GraphicsTexture) {   text }; 
	mat.shader = prog.programId; 
	box->location = (Vec4) {-5.0f,2.0f,-2.0f, 1.0f};
	GraphicsObjectSetGeom(box, md2model->info.md2.model.geom);
	GraphicsSceneAddObject(scene, box);	
	box->material = &mat2; 
	box->scale = (Vec4) { 0.005f, 0.005f,0.005f, 1.0f};
	

	GraphicsObject *box2 = GraphicsObjectCreate();
	box2->material = &mat;
	box2->location = (Vec4) {5.0f, 1.0f,0.0f,1.0f};
	box2->scale = (Vec4) { 3.0f, 3.0f, 3.0f, 1.0f};
	GraphicsObjectSetGeom(box2, GraphicsGeomCreateBox());
	GraphicsSceneAddObject(scene, box2);

	GraphicsObject *box3 = GraphicsObjectCreate();
	box3->material = &mat;
	box3->location = (Vec4) {6.0f, 0.0f,3.0f,1.0f};
	box3->scale = (Vec4) { 2.0f, 19.0f, 9.0f, 1.0f};
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
	plane->scale = (Vec4) { 100.0f, 0.20f, 100.0f, 1.0f};
	plane->location = (Vec4) { 5.0f, 0.0f,  5.0f, 1.0f};
	GraphicsObjectSetGeom(plane, GraphicsGeomCreateBox());
	GraphicsSceneAddObject(scene, plane);

	scene->cameraLocation = (Vec4) {   -50.0f,  75.0f,-50.0f, 1.0f};
	scene->cameraLookat = (Vec4) {0.0f, 0.0f,0.0f,1.0f};
	scene->directionalLight.direction = (Vec4) {8.0f, 8.0f,  8.0f,1.0f};
	scene->directionalLight.color = (Vec4) { 0.0210f, 0.0210f, 0.0210f, 1.0f};

	
	scene->directionalLight.direction = (Vec4) {9.0f, 18.0f, 3.0f,1.0f};
	scene->directionalLight.color = (Vec4) { 0.0021f, 0.0021f, 0.0031f, 1.0f};
	scene->pointLights[0].color = (Vec4) { 8.81f, 0.021f, 0.021f, 1.0f};
	scene->pointLights[0].position = (Vec4) { -5.0f, 5.0f, 1.0f, 1.0f};
	scene->pointLights[1].color = (Vec4) { 16.81f, 28.21f, 0.21f, 1.0f};
	scene->pointLights[1].position = (Vec4) { -15.0f, 5.0f, 5.0f, 1.0f};

	
	srand(time(NULL));

	for (int i =2 ; i < 16; i++)
	{
		float r = 8.0f * (float)rand()/(float)RAND_MAX;
		float q =  2.0f *(float)rand()/(float)RAND_MAX;
		 scene->pointLights[i].color = (Vec4) { 8.0f + r, 8.0f      , 1.0f - r  , 1.0f };
		 scene->pointLights[i].position = (Vec4) { 16.0f*(i-8), 6.5f, 11.0f, 1.0f};
	}
 
	int i = 0;
 	while(running)
	{  
		state = IOUpdate(state);
		running = state.running;  
		
		
		GraphicsRenderScene(scene);

		float t = (float)(clock()) / 1000.0f;  
		float d = 2.0 +  sin(t*10);
  
		box->rotationAngle = -3.142f / 2.0f;
		box->rotation = (Vec4) {1.0f, 0.0f, 0.0f, 1.0f};
 
		Rectangle from = { 0.0f, 0.0f, 1.0f, 1.0f};
		Rectangle to = { 0.0f, 0.0f, 500.0f, 500.0f};

		//2d
		Graphics2DDrawTexture(scene, text, from, to);
		Graphics2DDrawString(scene,fontTexture, fontRes, "ALPHA 0.0");

		scene->pointLights[1].color = (Vec4) { d*12.81f, d*45.21f, d*5.21f, 1.0f};
		scene->pointLights[1].position = (Vec4) { 30.0f * sin(t), 30.0f, 30.0f * cos(t), 1.0f};


		i++;
		box->offset = ( box->geom->numTris * 3 * ( i /  40) ); 

		if ( i > 4000)
			i = 0;
	}
	
 	SDL_Quit(); 
	return 0;
}