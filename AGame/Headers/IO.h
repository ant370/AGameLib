
#ifndef _IO_
#define _IO_

#include "SDL.h"
#include <stdbool.h>

#define IO_GAME_TITLE "Test Game"
#define IO_GAME_VERSION "1.0"
#define IO_DEFAULT_SCREEN_WIDTH 1200
#define IO_DEFAULT_SCREEN_HEIGHT 900
#define IO_MAX_RESOURCE_FILES 2024


struct IOState 
{
    SDL_Surface* surface;
    SDL_Window* window;
    bool running;

};

struct IOFile
{
    char * name;
    char * path;
    char * filetype;
    int size;
};
struct IOFiles
{   
    int count;
    struct IOFile * files;
};


 


char * IOLoadFile(char *fileName);
struct IOState IOInit(void);
struct IOFiles * IOFileFinder();
struct IOState IOUpdate( struct IOState state);



 
#endif