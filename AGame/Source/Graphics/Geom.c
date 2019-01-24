#include "Graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


GraphicsGeom *GraphicsGeomCreateTriangle()
{ 

    GraphicsGeom *geom = malloc(sizeof(GraphicsGeom));
    geom->verts = malloc( sizeof(VertexPositionTexture) * 3); 
    geom->verts[0] = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, 0.0f, 0.0f} }; 
    geom->verts[1] = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, 0.0f, 0.0f} }; 
    geom->verts[2] = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, 0.0f, 0.0f} }; 

    geom->index = malloc( sizeof(unsigned int) * 3);
    geom->index[0] = 0;
    geom->index[1] = 1;
    geom->index[2] = 2;

    geom->numVerts = 3; // 3 triangles
    geom->numTris = 3;

    return geom;    
}


GraphicsGeom *GraphicsGeomCreateCircle(int n)
{
    GraphicsGeom *geom = malloc(sizeof(GraphicsGeom));
    geom->verts = malloc( sizeof(VertexPositionTexture) * (n + 1)); 


    geom->verts[0] = (VertexPositionTexture ) { .Position = { 0.0f,  0.0, 0.0f, 0.0f} }; 

    for ( int j = 0; j < n; j++)
    {
        float p = 2.0f * 3.14159f * ((float) j / (float) n); 
        geom->verts[j+1] = (VertexPositionTexture ) { .Position = { sin(p),  cos(p), 0.0f, 0.0f} }; 
    }
 
    
    int numTris = n+1;

    geom->index = malloc( sizeof(unsigned int) * 3*(numTris + 1));
    for (int i = 0; i < n; i++)
    {
        
        geom->index[3*i + 0] = 0;
        geom->index[3*i + 1] = i+1;
        geom->index[3*i + 2] = i+2;
    } 
    
    geom->index[3*numTris + 0] = 0;
    geom->index[3*numTris + 1] = n;
    geom->index[3*numTris + 2] = 1;

    geom->numVerts = n + 1;
    geom->numTris = numTris + 1;

    return geom;    
}

GraphicsGeom *GraphicsGeomCreatePlaneGivenPoints(Vec4 a, Vec4 b, Vec4 c, Vec4 d)
{
    GraphicsGeom *geom = malloc(sizeof(GraphicsGeom));
    geom->verts = malloc( sizeof(VertexPositionTexture) * 4); 
    geom->verts[0] = (VertexPositionTexture ) { .Position = a, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[1] = (VertexPositionTexture ) { .Position = b, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[2] = (VertexPositionTexture ) { .Position = c, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[3] = (VertexPositionTexture ) { .Position = d, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 

    geom->index = malloc( sizeof(unsigned int) * 6);
    geom->index[0] = 0;
    geom->index[1] = 1;
    geom->index[2] = 2;
    geom->index[3] = 0;
    geom->index[4] = 2;
    geom->index[5] = 3;

    geom->numVerts = 4; // 3 triangles
    geom->numTris = 2;

    return geom;    
}

GraphicsGeom *GraphicsGeomCreatePlane()
{
    GraphicsGeom *geom = malloc(sizeof(GraphicsGeom));
    geom->verts = malloc( sizeof(VertexPositionTexture) * 4); 
    geom->verts[0] = (VertexPositionTexture ) { .Position = { 1.0f,  0.0f, 1.0f, 1.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f}, .Normal = {0.0f, 0.0f, 1.0f, 1.0f} }; 
    geom->verts[1] = (VertexPositionTexture ) { .Position = { 1.0f,  -0.0f, -1.0f, 1.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f}, .Normal = {0.0f, 0.0f, 1.0f, 1.0f} }; 
    geom->verts[2] = (VertexPositionTexture ) { .Position = { -1.0f,  -0.0f, -1.0f, 1.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f}, .Normal = {0.0f, 0.0f, 1.0f, 1.0f} }; 
    geom->verts[3] = (VertexPositionTexture ) { .Position = { -1.0f,  0.0f, 1.0f, 1.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f}, .Normal = {0.0f, 0.0f, 1.0f, 1.0f} }; 

    geom->index = malloc( sizeof(unsigned int) * 6);
    geom->index[0] = 1;
    geom->index[1] = 0;
    geom->index[2] = 3;
    geom->index[3] = 1;
    geom->index[4] = 3;
    geom->index[5] = 2;

    geom->numVerts = 4; // 3 triangles
    geom->numTris = 2;
    GraphicsGeomCalcNormals(geom);
    
    return geom;    
}

VertexPositionTexture GraphicsVertexAverage(const VertexPositionTexture a, const VertexPositionTexture b)
{
    const VertexPositionTexture vpt = {
        .Position = { (a.Position.x + b.Position.x ) /2.0, (a.Position.y + b.Position.y ) /2.0, (a.Position.z + b.Position.z ) /2.0, (a.Position.w + b.Position.w ) /2.0 },
        .Texture = { (a.Texture.x + b.Texture.x ) /2.0, (a.Texture.y + b.Texture.y ) /2.0, (a.Texture.z + b.Texture.z ) /2.0, (a.Texture.w + b.Texture.w ) /2.0 },
        .Normal =  { (a.Normal.x + b.Normal.x ) /2.0, (a.Normal.y + b.Normal.y ) /2.0, (a.Normal.z + b.Normal.z ) /2.0, (a.Normal.w + b.Normal.w ) /2.0 }
    }; 
    return vpt;
}

GraphicsGeom * GraphicsGeomSubDivTriangles(GraphicsGeom *geom, unsigned int n)
{

    GraphicsGeom * newGeom = malloc(sizeof(GraphicsGeom));
 
    unsigned int newTris = geom->numTris * 4; 
    unsigned int newVerts =  (newTris / 4) * 6 ; 

    newGeom->verts = malloc( sizeof(VertexPositionTexture) * newVerts); 
    newGeom->index = malloc( sizeof(unsigned int) * newTris* 3);
    newGeom->numVerts = newVerts;
    newGeom->numTris = newTris;

     unsigned int c = 0;
    for (int i = 0; i < geom->numTris  ; ++i)
    {
        unsigned int v0 = geom->index[3*i + 0];
        unsigned int v1 = geom->index[3*i + 1];
        unsigned int v2 = geom->index[3*i + 2];

        VertexPositionTexture vp01 = GraphicsVertexAverage( geom->verts[v0], geom->verts[v1]);
        VertexPositionTexture vp12 = GraphicsVertexAverage( geom->verts[v1], geom->verts[v2]);
        VertexPositionTexture vp02 = GraphicsVertexAverage( geom->verts[v0], geom->verts[v2]);


        newGeom->verts[i*6 + 0] = geom->verts[v0];
        newGeom->verts[i*6 + 1] = vp01;
        newGeom->verts[i*6 + 2] = vp02;
        
        newGeom->verts[i*6 + 3] = geom->verts[v1];
        newGeom->verts[i*6 + 4] = vp12;
        newGeom->verts[i*6 + 5] = geom->verts[v2];

        newGeom->index[c++] = i*6 + 0;
        newGeom->index[c++] = i*6 + 1;
        newGeom->index[c++] = i*6 + 2;

        
        newGeom->index[c++] = i*6 + 1;
        newGeom->index[c++] = i*6 + 3;
        newGeom->index[c++] = i*6 + 4;

        
        newGeom->index[c++] = i*6 + 1;
        newGeom->index[c++] = i*6 + 4;
        newGeom->index[c++] = i*6 + 2;

        
        newGeom->index[c++] = i*6 + 2;
        newGeom->index[c++] = i*6 + 4;
        newGeom->index[c++] = i*6 + 5;
    }

    if (n == 1)
    { 
        return newGeom;
    }
    else
    {
        GraphicsGeom * subdivAgain = GraphicsGeomSubDivTriangles(newGeom, n - 1);
        GraphicsGeomFree(newGeom);
        return subdivAgain;
    }


}
 

GraphicsGeom *GraphicsGeomSphere(unsigned int n)
{
    GraphicsGeom * box = GraphicsGeomCreateBox();
    GraphicsGeom * subDivBox = GraphicsGeomSubDivTriangles(box, n);
    GraphicsGeomFree(box); // Free Original box

    for (int i = 0; i < subDivBox->numVerts; i++)
    {
        float x = 2.0f *  subDivBox->verts[i].Position.x;
        float y = 2.0f *  subDivBox->verts[i].Position.y;
        float z = 2.0f *  subDivBox->verts[i].Position.z;

        //http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html

        float X = x * sqrt(1 - y*y*0.5 -z*z*0.5 + y*y*z*z*0.333333333);
        float Y = y * sqrt(1- z*z*0.5 - x*x*0.5 + z*z*x*x*0.333333333);
        float Z = z * sqrt(1- x*x*0.5 - y*y*0.5 + x*x*y*y*0.333333333);

        subDivBox->verts[i].Position.x = X;
        subDivBox->verts[i].Position.y = Y;
        subDivBox->verts[i].Position.z = Z;

        //Just normalise
        subDivBox->verts[i].Normal = Vec4Normal(subDivBox->verts[i].Position); 
    } 
    
    GraphicsGeomCalcNormals(subDivBox); 
    return subDivBox; 
}


GraphicsGeom * GraphicsGeomCreateBox()
{
     GraphicsGeom *geom = malloc(sizeof(GraphicsGeom));
    geom->verts = malloc( sizeof(VertexPositionTexture) * 24); 

    geom->verts[0]  = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, 0.5f, 0.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[1]  = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, 0.5f, 0.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[2]  = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, 0.5f, 0.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[3]  = (VertexPositionTexture ) { .Position = { -0.5f,  0.5f, 0.5f, 0.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 
    
    geom->verts[4]  = (VertexPositionTexture ) { .Position = { -0.5f,  0.5f, 0.5f, 0.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[5]  = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, 0.50f, 0.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[6]  = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, -0.50f, 0.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[7]  = (VertexPositionTexture ) { .Position = { -0.5f,  0.5f, -0.50f, 0.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 

    geom->verts[8]  = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, -0.5f, 0.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[9]  = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, -0.5f, 0.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[10] = (VertexPositionTexture ) { .Position = { -0.5f,  0.5f, -0.5f, 0.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[11] = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, -0.5f, 0.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 
    
    geom->verts[12] = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, -0.5f, 0.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[13] = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, -0.50f, 0.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[14] = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, 0.50f, 0.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[15] = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, 0.50f, 0.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 

    geom->verts[16] = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, -0.5f, 0.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[17] = (VertexPositionTexture ) { .Position = { -0.5f,  -0.5f, 0.5f, 0.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[18] = (VertexPositionTexture ) { .Position = { -0.5f,  0.5f, 0.5f, 0.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[19] = (VertexPositionTexture ) { .Position = { -0.5f,  0.5f, -0.5f, 0.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 
    
    geom->verts[20] = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, 0.5f, 0.0f}, .Texture = {1.0f,1.0f,0.0f,0.0f} }; 
    geom->verts[21] = (VertexPositionTexture ) { .Position = { 0.5f,  -0.5f, -0.50f, 0.0f}, .Texture = {1.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[22] = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, -0.50f, 0.0f}, .Texture = {0.0f,0.0f,0.0f,0.0f} }; 
    geom->verts[23] = (VertexPositionTexture ) { .Position = { 0.5f,  0.5f, 0.50f, 0.0f}, .Texture = {0.0f,1.0f,0.0f,0.0f} }; 

     float cube_vertices[] = {
                /// front
            -0.5, -0.5,  0.5,
            0.5, -0.5,  0.5,
            0.5,  0.5,  0.5,
            -0.5,  0.5,  0.5,
            // top
            -0.5,  0.5,  0.5,
            0.5,  0.5,  0.5,
            0.5,  0.5, -0.5,
            -0.5,  0.5, -0.5,
            // back
            0.5, -0.5, -0.5,
            -0.5, -0.5, -0.5,
            -0.5,  0.5, -0.5,
            0.5,  0.5, -0.5,
            // bottom
            -0.5, -0.5, -0.5,
            0.5, -0.5, -0.5,
            0.5, -0.5,  0.5,
            -0.5, -0.5,  0.5,
            // left
            -0.5, -0.5, -0.5,
            -0.5, -0.5,  0.5,
            -0.5,  0.5,  0.5,
            -0.5,  0.5, -0.5,
            // right
            0.5, -0.5,  0.5,
            0.5, -0.5, -0.5,
            0.5,  0.5, -0.5,
            0.5,  0.5,  0.5,
            };
    for ( int i = 0; i < 24; i++)
    {
        geom->verts[i].Position.x = cube_vertices[3*i + 0];
        geom->verts[i].Position.y= cube_vertices[3*i + 1];
        geom->verts[i].Position.z = cube_vertices[3*i + 2];
        geom->verts[i].Position.w = 2.0f;
    }



    geom->index = malloc( sizeof(unsigned int) * 36);
    unsigned int cube_elements[] = {
    // front
        0,  1,  2,
        2,  3,  0,
        // top
        4,  5,  6,
        6,  7,  4,
        // back
        8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
  }; 
    for (int i =0; i < 36; i ++)
    { 
        geom->index[i] = cube_elements[i];
    }
    
    
    geom->numTris = 12;
    geom->numVerts = 24;
    GraphicsGeomCalcNormals(geom); 
    return geom;
 
}

void  GraphicsGeomCalcNormals(GraphicsGeom *geom)
{
    for (int i = 0; i < geom->numTris; i++)
    {
        int v1 = geom->index[3*i + 0];
        int v2 = geom->index[3*i + 1];
        int v3 = geom->index[3*i + 2];

        VertexPositionTexture vp1 = geom->verts[v1];
        VertexPositionTexture vp2 = geom->verts[v2];
        VertexPositionTexture vp3 = geom->verts[v3];

        Vec4 a = Vec4Sub(vp2.Position, vp1.Position);
        Vec4 b = Vec4Sub(vp3.Position, vp1.Position);
        Vec4 normal = Vec4Normal(Vec4Cross(a,b));

        geom->verts[v1].Normal = normal;
        geom->verts[v2].Normal = normal;
        geom->verts[v3].Normal = normal;
    }
 
}
 

double clamp(double x, double upper, double lower)
{ 
    return x;
    if ( x > upper)
        return upper;
    if ( x < lower)
        return upper;
        
}


void GraphicsGeomFree(GraphicsGeom *geom)
{ 
    free(geom->index);
    free(geom->verts);
    free(geom);
}

