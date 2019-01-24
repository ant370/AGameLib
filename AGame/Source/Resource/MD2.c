#include "Resource.h"
#include <stdio.h>


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

    md2_header_t * header = ( md2_header_t *) resc->data ;

    char * skins_c = resc->data + header->offset_skins;

    md2_skin_t * skins = (md2_skin_t *) (resc->data + header->offset_skins);
    md2_texCoord_t * text = (md2_texCoord_t * ) resc->data + header->offset_st;
    md2_triangle_t * triangles = (md2_triangle_t *) resc->data + header->offset_tris;
    md2_frame_t * frams = (md2_frame_t *) (resc->data + header->offset_frames);
    
    printf("frames: %f %f %f %s  \n ", frams->scale[0], frams->scale[1], frams->scale[2], frams->name);
 
    return 0;
}