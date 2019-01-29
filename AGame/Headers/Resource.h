
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
        struct {
            int width;
            int height;
        } image;
        struct {
            GameResourceMD2Model model;
        } md2;
        struct 
        {
            stbtt_bakedchar * data;
            unsigned char * bitmap;
        } font;
    } info;

} GameResource;


GameResource * GameResourceLoadFile(char * file); 
GameResource * GameResourceLoadPNG(char * file); 
GameResource * GameResourceLoadMD2(char * file); 
GameResource * GameResourceLoadFont(char * file, int height);

void GameResourceFree(GameResource * resc);  
void GameResourceManifestLoad(char * file);


#endif