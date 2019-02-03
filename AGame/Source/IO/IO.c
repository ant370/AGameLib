

#include "IO.h"
#include "SDL.h" 
#include "glew.h"

#include <stdio.h> 
#include <string.h>



#define CUTE_FILES_IMPLEMENTATION
#include "cute_files.h"

struct IOState IOInit(void)
{
    SDL_Window* window = NULL;
        
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    { 
    } 
    //Create window
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                IO_DEFAULT_SCREEN_WIDTH, IO_DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if( window == NULL )
    { 
        
    } 

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    SDL_GLContext openglContext = SDL_GL_CreateContext(window); 
     
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();

    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }

    struct IOState state = 
    {
        .surface = screenSurface,
        .window = window,
        .running = true
    };
    SDL_GL_SetSwapInterval(0);

    glEnable(GL_TEXTURE);

    return state;
}

struct IOState IOUpdate( struct IOState state)
{  
    SDL_GL_SwapWindow(state.window);
    //Event handler
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT )
        { 
            state.running = false;
        }
    }

    SDL_UpdateWindowSurface( state.window );  
    //glClearColor(1,0,0,1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return state; 
}
 

char* IOLoadFile(char *fileName)
{
    char *source = NULL;
    FILE *fp = fopen(fileName, "r");
    if (fp != NULL) 
    {
        printf ("OPENED file %s", fileName);
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) 
        {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0'; /* Just to be safe. */
            }
        }
        fclose(fp);
    } 
    else
    {
        printf("FAILED to open file %s", fileName);
    }

    return source;
}

void IOPrintFile(cf_file_t* file, void* udata);
 
void IOPrintFile(cf_file_t* file, void* udata)
{
    struct IOFiles * files = (struct IOFiles *) udata; 

    files->files[files->count].name = malloc(strlen(file->name) + 1);
    files->files[files->count].path = malloc(strlen(file->path) + 1);
    files->files[files->count].filetype = malloc(strlen(file->ext) + 1);

    strcpy(files->files[files->count].name,file->name); 
    strcpy(files->files[files->count].path,file->path);
    strcpy(files->files[files->count].filetype,file->ext);

    files->files[files->count].size = file->size;
    files->count = files->count + 1;

    
    //Ensure that max files is not exceeded.
    assert(files->count != IO_MAX_RESOURCE_FILES);
    printf("%s \n", files->files[files->count-1].path);
}

struct IOFiles * IOFileFinder()
{ 
    struct IOFiles * files = malloc(sizeof(struct IOFiles)); 
    files->count =0;
    files->files = malloc(sizeof(struct IOFile) * IO_MAX_RESOURCE_FILES);

    cf_traverse("Resources", &IOPrintFile, files); 

    return files;
}