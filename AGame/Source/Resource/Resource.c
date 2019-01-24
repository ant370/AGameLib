
#include "Resource.h"
#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

GameResource * GameResourceLoadFile(char * file)
{
    GameResource * resource = malloc(sizeof(GameResource));


    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen(file, "rb");  // Open the file in binary mode

    if(fileptr == NULL)
        printf("no file pointer");

    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file

    resource->data = buffer;
    resource->id = 0;
    resource->isLoaded = 1;
    resource->name = file;
    resource->resourceType = GameResourceRawData;
    resource->size = filelen + 1;

    printf("resource loaded");
    return resource;
}


GameResource * GameResourceLoadPNG(char * file)
{ 
    GameResource * resc = GameResourceLoadFile(file);

    unsigned error;
    unsigned char* image;
    unsigned width, height;
    unsigned char* png = 0;
    size_t pngsize;
 
    if(!error) error = lodepng_decode32(&image, &width, &height, resc->data, resc->size);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  
    GameResourceFree(resc); // Don't need the original resource

    GameResource * newresc = malloc(sizeof(GameResource));
    newresc->data = image;
    newresc->info.image.height = height;
    newresc->info.image.width = width;
    newresc->size = -1;
    newresc->resourceType = GameResourceImagePNG;
    newresc->isLoaded = 1;
    newresc->id = -1;

    printf ("LOADED IMAGE %i %i %s", width, height, file);

    return newresc;
}


void GameResourceFree(GameResource * resc)
{
    // free data and resc
    free (resc->data);
    free (resc);
}