
#include "Resource.h" 
#include "khash.h"
#include "IO.h"

KHASH_MAP_INIT_STR(resources, GameResource *)

khash_t(resources) * global_resources;
struct IOFiles * global_files;

void GameResourceLoadAll()
{
    for (int i = 0; i < global_files->count; i++)
    {
        
        if(!strcmp(global_files->files[i].filetype, ".md2"))
        {
            GameResource *resc = GameResourceLoadMD2(global_files->files[i].path);
            GameResourceSetDict(global_files->files[i].name, resc);
        }
        else if(!strcmp(global_files->files[i].filetype, ".ttf"))
        { 
            GameResource *resc = GameResourceLoadFont(global_files->files[i].path, 32);
            GameResourceSetDict(global_files->files[i].name, resc);
        }
        else if(!strcmp(global_files->files[i].filetype, ".png"))
        { 
            GameResource *resc = GameResourceLoadPNG(global_files->files[i].path);
            GameResourceSetDict(global_files->files[i].name, resc);
        }
        else if(!strcmp(global_files->files[i].filetype, ".ogg"))
        { 
            GameResource *resc = GameResourceLoadOggVorbis(global_files->files[i].path);
            GameResourceSetDict(global_files->files[i].name, resc);
        }
    }
}

void GameResourceInit()
{
    global_resources = kh_init(resources); 

    //Load all files
	global_files = IOFileFinder();
    GameResourceLoadAll();

}

void GameResourceSetDict(char * val, GameResource * resc)
{

    int absent = 0;
    khint_t k;
    k = kh_put(resources, global_resources, val, &absent);
    kh_value(global_resources, k) = resc;  
}

void GameResourceLogResources()
{
    printf(" Resources: %i", kh_size(global_resources));
    
    khint_t k;
    for (k = kh_begin(global_resources); k != kh_end(global_resources); ++k)   
    {
        
        if (kh_exist(global_resources, k))
        {
            GameResource * resc = kh_value(global_resources, k);
            printf("\n Resc: %s %i             ", kh_key(global_resources, k), resc->size);
        }     
    } 
}

GameResource * GameResourceAvailable(char * identity)
{
    khint_t k = kh_get_resources(global_resources,identity );
    GameResource * resc =  kh_value(global_resources, k );
    return resc;
}
