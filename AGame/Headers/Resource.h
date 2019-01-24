
#ifndef RESOURCE_H
#define RESOURCE_H

typedef enum GameResourceType
{
    GameResourceManifest,
    GameResourceRawData,
    GameResourceImagePNG

} GameResourceType;

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
    } info;

} GameResource;
 

GameResource * GameResourceLoadFile(char * file); 
GameResource * GameResourceLoadPNG(char * file); 
GameResource * GameResourceLoadMD2(char * file); 

void GameResourceFree(GameResource * resc); 

void GameResourceManifestLoad(char * file);


#endif