
#ifndef ANT_GRAPHICS_H
#define ANT_GRAPHICS_H

#include "Types.h"
#include <math.h>
#include "Resource.h"
#include <stdbool.h>

 
unsigned int GraphicsCreateVBO();
unsigned int GraphicsVBOTransfer(unsigned int vbo,int size, VertexPositionTexture * verts);
unsigned int GraphicsCreateVAO(int size, VertexPositionTexture * verts);

unsigned int GraphicsCreateVAOFromGeom(GraphicsGeom * geom);

/// OpenGL Shaders
GraphicsShaderProgram GraphicsCreateShader(char *vertexSource, char *fragSource);
void GraphicsShaderSetUniformFloat(unsigned int program, char *name, float val );
void GraphicsShaderSetUniformMatrix(unsigned int program, char *name, Matrix m );

/// DIRTY TEMP CODE
void createAndDrawTriangle();
 
/* *  GEOMETRY CREATE   */
GraphicsGeom *GraphicsGeomCreateTriangle(); 
GraphicsGeom *GraphicsGeomCreatePlane(); 
GraphicsGeom *GraphicsGeomCreateCircle(int n);
GraphicsGeom *GraphicsGeomCreateBox();

/* *  GEOMETRY CREATE   */
GraphicsGeom *GraphicsGeomSphere(unsigned int n); 
GraphicsGeom * GraphicsGeomSubDivTriangles(GraphicsGeom *geom, unsigned int n);

/* *  GEOMETRY CLEANUP   */
void GraphicsGeomFree(GraphicsGeom *geom);

/** 
 * STRUCTURES
 * **/

/** Material **/

typedef struct GraphicsTexture { 
    unsigned int textureId;
} GraphicsTexture;

typedef struct GraphicsMaterial { 

    unsigned int shader;
    GraphicsTexture albido;

} GraphicsMaterial;


unsigned int  GraphicsGenerateCheckerTexture(int patterns); 
unsigned int GraphicsGenerateUniformTexture(int patterns, int value);
unsigned int GraphicsGenerateTextureFromImage(char *data, int width, int height); 
unsigned int GraphicsGenerateTextureFromFont(char * data, int width, int height);
/**
 * Lights
 * 
 **/

typedef struct GraphicsPointLight
{
    Vec4 position;
    Vec4 color;
    
}  GraphicsPointLight;

 
typedef struct GraphicsDirectionalLight
{ 
    Vec4 direction;
    Vec4 color;
} GraphicsDirectionalLight;



/**
 * 
 * Scene
 * 
 */
typedef struct GraphicsObject
{
    GraphicsMaterial *material;
    GraphicsGeom *geom;
    Vec4 location;
    Vec4 rotation;
    Vec4 scale;
    float rotationAngle;
    unsigned int vao;
    unsigned int offset;

} GraphicsObject;


#define GRAPHICS_SCENE_MAX_OBJECTS 1000
#define GRAPHICS_POINT_LIGHTS 16
typedef struct GraphicsScene
{
    GraphicsObject * objects[GRAPHICS_SCENE_MAX_OBJECTS];
    int objectsCount;

    

    Vec4 cameraLocation;
    Vec4 cameraLookat;

    float screenWwidth;
    float screenHeight;

    GraphicsDirectionalLight directionalLight;
    GraphicsPointLight pointLights[GRAPHICS_POINT_LIGHTS];

    unsigned int shadowBuffer;
    unsigned int shadowBufferTexture;
    unsigned int shadowBufferWidth;
    unsigned int shadowBufferHeight;
    GraphicsShaderProgram shadowProgram;

    //Renders a depth map to a quad
    GraphicsShaderProgram depthShaderProgram; 
    GraphicsShaderProgram basic2dProgram;


    //G Buffers 
    unsigned int gBuffer; 
    unsigned int gPosition;
    unsigned int gNormal;
    unsigned int  gAlbedoSpec;
    
    ///FLAGS
    int shadowsEnabled;
    
} GraphicsScene;

GraphicsScene * GraphicsSceneCreate();
void GraphicsSceneAddObject(GraphicsScene * scene, GraphicsObject * obj); 
GraphicsObject * GraphicsObjectCreate();
void GraphicsObjectSetGeom(GraphicsObject * obj, GraphicsGeom * geom);

void GraphicsShaderSetUniformVec4(unsigned int program, char *name, Vec4 vec );
void GraphicsRenderScene(GraphicsScene *scene);
void GraphicsRenderDepthTexture(GraphicsScene *scene);
void GraphicsRenderTexture(GraphicsScene *scene, unsigned int texture); 
void GraphicsGeomCalcNormals(GraphicsGeom *geom);

void Graphics2DDrawTexture(GraphicsScene *scene, unsigned int texture, Rectangle from, Rectangle to);
void Graphics2DDrawString(GraphicsScene *scene, unsigned int texture, GameResource *font, char * str);

#endif
