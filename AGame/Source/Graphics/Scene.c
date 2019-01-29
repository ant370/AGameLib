
#include "Graphics.h"
#include "IO.h"

GraphicsScene * GraphicsSceneCreate()
{
    GraphicsScene *scene = malloc(sizeof(GraphicsScene));
    scene->objectsCount = 0;
    scene->directionalLight.direction = (Vec4) {0.0f, -1.0f, 0.0f, 0.0f};
    scene->directionalLight.color = (Vec4) {1.0f, 1.0f, 1.0f,0.0f};
    
    //Shadow Program.
    char *vertexShader = R"(
        #version 330 core 
        layout (location = 0) in vec4 aPos; 
        layout (location = 1) in vec4 aText;
        layout (location = 2) in vec4 aNormal;
        uniform mat4 project;
        uniform mat4 view;
        uniform mat4 model;

        void main()
        {
            gl_Position = vec4(aPos.xyz, 1.0) * model  * view * project  ;
        }  

    )";

    char *fragShader = R"(
        
        #version 330 core

        void main()
        {              
        } 

    )";


    char * basicsVert = IOLoadFile("Shaders/guivert.glsl");
    char * basicsFrag = IOLoadFile("Shaders/guifrag.glsl");
    scene->basic2dProgram = GraphicsCreateShader(basicsVert, basicsFrag);


    scene->shadowProgram = GraphicsCreateShader(vertexShader, fragShader);
    scene->shadowsEnabled = 1; 
    scene->shadowBuffer = -1;


    char * depthVert = IOLoadFile("Shaders/depthvert.glsl");
    char * depthFrag = IOLoadFile("Shaders/depthfrag.glsl");
    scene->depthShaderProgram = GraphicsCreateShader(depthVert, depthFrag);


    scene->screenHeight = 900.0f;
    scene->screenWwidth = 1200.0f;
    return scene;
}

GraphicsObject * GraphicsObjectCreate()
{
    GraphicsObject * obj = malloc(sizeof(GraphicsObject));
    obj->location = (Vec4) {0.0f, 0.0f, 0.0f, 0.0f};
    obj->rotation = (Vec4) { 0.0f, 1.0f, 0.0f, 0.0f};
    obj->rotationAngle = 0.0f;
    obj->scale = (Vec4) {1.0f, 1.0f, 1.0f,0.0f};
    
    return obj;
}


void GraphicsObjectSetGeom(GraphicsObject * obj, GraphicsGeom * geom)
{
    obj->geom = geom;
    
    //Create vertex array object
    unsigned int vao = GraphicsCreateVAOFromGeom(geom);
    obj->vao = vao;
}

void GraphicsSceneAddObject(GraphicsScene * scene, GraphicsObject *obj)
{
    if (scene->objectsCount - 1 == GRAPHICS_SCENE_MAX_OBJECTS)
    {
        //error 
    }
    else
    {
        scene->objects[scene->objectsCount] = obj;
        scene->objectsCount = scene->objectsCount + 1; 
    }
}