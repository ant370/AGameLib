
#ifndef ANT_AUDIO
#define ANT_AUDIO

#include "Types.h"
#include "Resource.h" 
#include "AL/al.h"  
#include "AL/alc.h"

///
/// Audio System
/// Thread safe, simple call to AudioPlay should handle all the details
/// 

typedef struct AudioSystem
{ 
    unsigned int maxAudioResources; 
    unsigned int * alBufferId;
    
    /* data */
    ALCdevice *device;
    ALCcontext *context;

} AudioSystem;


AudioSystem * AudioInit();
void AudioPlay(AudioSystem * sys, unsigned int audioId, Vec4 position);
unsigned int AudioResource(AudioSystem * sys, unsigned int audioId, const GameResource * resource);

void AudioUnloadResource(AudioSystem * sys, const GameResource * resource);
void AudioListnerUpdate(AudioSystem * sys,Vec4 position, Vec4 velocity);

#endif