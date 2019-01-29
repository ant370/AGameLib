
#ifndef RESOURCE_H
#define RESOURCE_H
 
#include "Types.h"
#include "stb_truetype.h"

typedef enum GameResourceType
{
    GameResourceManifest,
    GameResourceRawData,
    GameResourceImagePNG,
    GameResourceFont

} GameResourceType;

typedef struct GameResourceBitmapFontFile
{
    char * info_face;

} GameResourceBitmapFontFile;

typedef struct GameResourceMD2Model
{
    GraphicsGeom * geom;

} GameResourceMD2Model;

typedef struct GameResource
{
    unsigned int id;
    unsigned int resourceType;
    unsigned int size;
    unsigned int isLoaded;
    char * name;
    char * data;

    union
    { 
        //Image file format
        struct {
            int width;
            int height;
        } image;

        //Model - Animated 3d vertex MD2
        struct {
            GameResourceMD2Model model;
        } md2;


        struct 
        {
            stbtt_bakedchar * data;
            unsigned char * bitmap;
        } font;

        struct 
        {
            int channels;
            int length;
            int samples;
            short * data;
        } sound;
    } info;

} GameResource;


GameResource * GameResourceLoadFile(char * file); 
GameResource * GameResourceLoadPNG(char * file); 
GameResource * GameResourceLoadMD2(char * file); 
GameResource * GameResourceLoadFont(char * file, int height);
GameResource * GameResourceLoadOggVorbis(char * file );

//TODO:
// COMPREHNESIVE MEMMORY MANAGEMENT
// TYPES for RESOURCES rather than assumption

void GameResourceFree(GameResource * resc);  
void GameResourceManifestLoad(char * file);


#endif