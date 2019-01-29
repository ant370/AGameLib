
#include "Resource.h"
#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h" 

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

#include "stb_vorbis.c"

#

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

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];

stbtt_bakedchar cdata[96]; //

GameResource * GameResourceLoadFont(char * file, int height)
{
    
    GameResource * data = GameResourceLoadFile(file);

    GameResource * resc = malloc(sizeof(GameResource));
    resc->info.font.data = malloc(sizeof(stbtt_bakedchar) * 196);
    resc->info.font.bitmap = malloc(512*512*10);
    resc->id = -1;
    resc->size = 4;



    stbtt_BakeFontBitmap(data->data, 0, height, resc->info.font.bitmap,
                512,512, 32,96, resc->info.font.data); // no guarantee this fits!
   
     
    GameResourceFree(data);
    return resc;
}


GameResource * GameResourceLoadOggVorbis(char * file )
{
    GameResource * data = GameResourceLoadFile(file);
    
    int channels = -1;
    int sample = -1;
    short *decoded;
    
    int i = stb_vorbis_decode_memory(data->data, data->size, 
        &channels, &sample, &decoded);

    data->data = NULL;
    data->info.sound.data = decoded;
    data->info.sound.channels = channels;
    data->info.sound.samples = sample;
    data->info.sound.length = i;
    
    //free the initial file memmory
    free(data->data); //No longer need that..

    //Return the data
    return data;
}
 