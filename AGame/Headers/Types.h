#ifndef TYPES_ANT
#define TYPES_ANT

typedef float VecFloat;

/** Math Types **/

typedef struct Vec4
{
	VecFloat x;
	VecFloat y;
	VecFloat z;
	VecFloat w;

} Vec4;

typedef struct Matrix
{
	VecFloat a_0_0;
	VecFloat a_0_1;
	VecFloat a_0_2;
	VecFloat a_0_3;
	VecFloat a_1_0;
	VecFloat a_1_1;
	VecFloat a_1_2;
	VecFloat a_1_3;
	VecFloat a_2_0;
	VecFloat a_2_1;
	VecFloat a_2_2;
	VecFloat a_2_3;
	VecFloat a_3_0;
	VecFloat a_3_1;
	VecFloat a_3_2;
	VecFloat a_3_3;

} Matrix;

/** Graphics Type **/

 typedef struct VertexPositionTexture 
 {
     Vec4 Position;
     Vec4 Texture;
     Vec4 Normal;
     Vec4 Color;
 } VertexPositionTexture;

typedef struct GraphicsShaderProgram
{
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int programId;
    int errorVertFlag;
    int errorFragFlag;
	
} GraphicsShaderProgram;

typedef struct GraphicsGeom
{
    unsigned int numTris;
    unsigned int numVerts;

    unsigned int * index;
    VertexPositionTexture * verts;
} GraphicsGeom;



//Rectangle
typedef struct Rectangle
{
	float x;
	float y;
	float width;
	float height;
} Rectangle;


#endif