
#ifndef _IO_
#define _IO_

#include "SDL.h"
#include <stdbool.h>

#define IO_GAME_TITLE "Test Game"
#define IO_GAME_VERSION "1.0"
#define IO_DEFAULT_SCREEN_WIDTH 1200
#define IO_DEFAULT_SCREEN_HEIGHT 900

/** *
 * IO.h "Input Output" is for platform specific input output.
 * **/ 

typedef struct IOState 
{
    SDL_Surface* surface;
    SDL_Window* window;
    bool running;

} IOState;



char * IOLoadFile(char *fileName);
IOState IOInit(void);
IOState IOUpdate(IOState state);
int getval();




 
#endif