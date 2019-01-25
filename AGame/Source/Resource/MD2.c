#include "Resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct md2_header_t
{
  int ident;                  /* magic number: "IDP2" */
  int version;                /* version: must be 8 */

  int skinwidth;              /* texture width */
  int skinheight;             /* texture height */

  int framesize;              /* size in bytes of a frame */

  int num_skins;              /* number of skins */
  int num_vertices;           /* number of vertices per frame */
  int num_st;                 /* number of texture coordinates */
  int num_tris;               /* number of triangles */
  int num_glcmds;             /* number of opengl commands */
  int num_frames;             /* number of frames */

  int offset_skins;           /* offset skin data */
  int offset_st;              /* offset texture coordinate data */
  int offset_tris;            /* offset triangle data */
  int offset_frames;          /* offset frame data */
  int offset_glcmds;          /* offset OpenGL command data */
  int offset_end;             /* offset end of file */
} md2_header_t;


typedef float vec3_t[3]; 

typedef struct md2_skin_t
{
  char name[64];              /* texture file name */
} md2_skin_t; 

typedef struct md2_texCoord_t
{
  short s;
  short t;
} md2_texCoord_t;

typedef struct md2_triangle_t
{
  unsigned short vertex[3];   /* vertex indices of the triangle */
  unsigned short st[3];       /* tex. coord. indices */
} md2_triangle_t;

/* Compressed vertex */
typedef struct md2_vertex_t
{
  unsigned char v[3];         /* position */
  unsigned char normalIndex;  /* normal vector index */
} md2_vertex_t;

typedef struct md2_frame_t
{
  vec3_t scale;               /* scale factor */
  vec3_t translate;           /* translation vector */
  char name[16];              /* frame name */
  struct md2_vertex_t *verts; /* list of frame's vertices */
} md2_frame_t;




GameResource * GameResourceLoadMD2(char * file)
{
    //Get data
    GameResource * resc = GameResourceLoadFile(file);

    GameResource * model = malloc(sizeof(GameResource));
    model->data = NULL;
    model->id = 0;
    model->isLoaded = 1;
    model->resourceType = 0; 

    //Transfer structured data
    md2_header_t *header = malloc(sizeof(md2_header_t));
    md2_triangle_t *triangles = malloc(sizeof(md2_triangle_t) * header->num_tris);
    md2_frame_t * frames; //no allocation needed
    md2_texCoord_t * texts = malloc(sizeof(md2_texCoord_t) * header->num_st);

    memcpy(header, resc->data, sizeof(md2_header_t)); 
    memcpy(triangles, resc->data + header->offset_tris,  sizeof(md2_triangle_t) * header->num_tris );
    memcpy(texts,  resc->data + header->offset_st, sizeof(md2_texCoord_t) * header->num_st);

    int size = header->num_frames * (12 + 12 + 16 + sizeof(md2_vertex_t) * header->num_vertices);
    char * frameData = malloc(size);
    memcpy(frameData, resc->data + header->offset_frames, size );

    printf(" %.64s ", frameData);


    printf("header %i \n", header->num_frames);
 
    for (int i = 0; i < header->num_frames; i++)
    {
      const int frameSize = header->framesize;
      const int offset = header->offset_frames;
        
      frames = (md2_frame_t *) frameData;
      frames =  (md2_frame_t *) ( frameData + frameSize*2);
    }

    
    printf(" %s ", frames->name);

    //Build geom
      //Creating Graphics Geometry - 
    GraphicsGeom * geom = malloc(sizeof(GraphicsGeom));
    geom->verts = malloc(sizeof(VertexPositionTexture) * header->num_frames * header->num_vertices * 3 * 4 * 100);
    geom->index = malloc(sizeof(unsigned int) * header->num_frames  * header->num_tris * 3 * 100 );
    geom->numTris = header->num_tris;
    geom->numVerts = header->num_tris * 3; 
    
    
    int index = 0;
    //Vertex
    for ( unsigned int i =0; i < header->num_frames; i++)
    {
      
      md2_frame_t frame = *  (md2_frame_t *) ( frameData + header->framesize*i);
 
      //For each triangle
      for (int j = 0; j < header->num_tris; j++)
      {   
          for (int k =0; k < 3; k++)
          {
            int vertex = triangles[j].vertex[k];
            md2_vertex_t vert = ((md2_vertex_t *)( frameData + header->framesize*i + 12 + 12 + 16))[vertex ];

            float x = (frame.scale[0] * (float)vert.v[0]) + frame.translate[0];
            float y = (frame.scale[1] * (float)vert.v[1]) + frame.translate[1];
            float z = (frame.scale[2] * (float)vert.v[2]) + frame.translate[2];

            float s = ((float)texts[triangles[j].st[k]].s)/((float) header->skinwidth);
            float t = ((float)texts[triangles[j].st[k]].t)/((float) header->skinheight);
          
            geom->verts[index] = (VertexPositionTexture) 
                { .Position = {x,y,z, 1.0f} ,
                  .Texture = {s,t,0.0f, 1.0f},
                  .Normal = {1.0f, 0.0f, 0.0f, 1.0f}};  

            //set geom as we go
            geom->index[index] = index ;
            index ++;

            //printf ("%f %f %f \n", x,y,z );
          }
      } 

    }


    printf("done verts");
    
  
    model->info.md2.model.geom = geom; 
    
    GameResourceFree(resc);

    //Cleanup resources
    free(header);
    free(triangles);
    free(texts);
    //free(frames);
    free(frameData);

    return model;
}