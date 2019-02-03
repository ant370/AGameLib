
#ifndef RESOURCE_H
#define RESOURCE_H
 
#include "Types.h"
#include "stb_truetype.h"
#include <stdbool.h>


typedef enum GameResourceType
{
    GameResourceManifest,
    GameResourceRawData,
    GameResourceImagePNG,
    GameResourceFont,
    GameResourceOgg,
    GameResourceMD2

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

struct GameResourceCache
{
    int resources;
};
 
GameResource * GameResourceLoadFile(char * file); 
GameResource * GameResourceLoadPNG(char * file); 
GameResource * GameResourceLoadMD2(char * file); 
GameResource * GameResourceLoadFont(char * file, int height);
GameResource * GameResourceLoadOggVorbis(char * file );

// CORE API
void GameResourceInit();
void GameResourceLogResources();
void GameResourceSetDict(char * val, GameResource * resc);
void GameResourceHint(char * identity);
void GameResourceNeed(char * identity);
int GameResourceLoadNeeded();
GameResource * GameResourceAvailable(char * identity);


//TODO:
// COMPREHNESIVE MEMMORY MANAGEMENT
// TYPES for RESOURCES rather than assumption

void GameResourceFree(GameResource * resc);  
void GameResourceManifestLoad(char * file);


#endif