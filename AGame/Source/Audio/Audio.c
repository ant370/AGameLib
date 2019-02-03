
#include "AntAudio.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "AL/al.h"
#include "AL/alc.h"

#define MAX_AUDIO_RESOURCES 1024


//Helper function - get format for AL
static inline ALenum to_al_format(short channels, short samples);
void AudioHandleError();

//Internal
ALuint AudioCreateOrigin(Vec4 position, float gain);


static void list_audio_devices(const ALCchar *devices)
{
        const ALCchar *device = devices, *next = devices + 1;
        size_t len = 0;

        fprintf(stdout, "Devices list:\n");
        fprintf(stdout, "----------\n");
        while (device && *device != '\0' && next && *next != '\0') {
                fprintf(stdout, "%s\n", device);
                len = strlen(device);
                device += (len + 1);
                next += (len + 2);
        }
        fprintf(stdout, "----------\n");
}
 


AudioSystem * AudioInit(){

    AudioSystem * asystem = malloc(sizeof(AudioSystem));
    asystem->maxAudioResources = MAX_AUDIO_RESOURCES; 
    asystem->alBufferId =  malloc(sizeof(unsigned int) * asystem->maxAudioResources);

    //OpenAL init 
    //Create device
    asystem->device = alcOpenDevice(NULL);
    if (asystem->device == NULL)
    {
        //Error handling undefined.
        //
        AudioHandleError(); // Error Handling
    }

    //Create Context
    asystem->context = alcCreateContext(asystem->device, NULL); 
    if (asystem->context == NULL)
    {
        // catastrophic error handlign undefined.
        AudioHandleError(); // Error Handling
    }

    alcMakeContextCurrent(asystem->context);

    list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    AudioHandleError(); // Error Handling
    return asystem;
}


void AudioPlay(AudioSystem * sys, unsigned int audioId, Vec4 position)
{ 
    unsigned int source = AudioCreateOrigin(position, 18.0f);
    
    AudioHandleError(); // Error Handling
    //bind it to the buffer
    alSourcei(source, AL_BUFFER, audioId);
    AudioHandleError(); // Error Handling
    alSourcePlay(source);
    AudioHandleError(); // Error Handling

    //Test code
    ALint source_state;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state); 
}

ALuint AudioCreateOrigin(Vec4 position, float gain)
{
    ALuint source; 
    alGenSources((ALuint)1, &source);
    AudioHandleError(); // Error Handling
    alSourcef(source, AL_PITCH, 1); 
    AudioHandleError(); // Error Handling
    alSourcef(source, AL_GAIN, 1);
    AudioHandleError(); // Error Handling
    alSource3f(source, AL_POSITION, position.x, position.y, position.z);
    AudioHandleError(); // Error Handling
    alSource3f(source, AL_VELOCITY, 0, 0, 0.10f);
    AudioHandleError(); // Error Handling
    alSourcei(source, AL_LOOPING, AL_TRUE);
    AudioHandleError(); // Error Handling

    return source;
}

ALuint AudioCreateBuffer(GameResource * resc)
{
    //Generate the buffer
    ALuint buffer;
    alGenBuffers((ALuint)1, &buffer); 
    AudioHandleError(); // Error Handling


    //Load from our game resource 
    alBufferData(buffer, to_al_format(resc->info.sound.channels, resc->info.sound.samples),
            resc->info.sound.data,
            resc->info.sound.length * sizeof(short) * resc->info.sound.channels ,
            resc->info.sound.samples);

    AudioHandleError(); // Error Handling

    return buffer;
}

void AudioListnerUpdate(AudioSystem * sys,Vec4 position, Vec4 velocity)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z );
}

static inline ALenum to_al_format(short channels, short samples)
{
        bool stereo = (channels > 1);
        return AL_FORMAT_STEREO16;
        switch (samples) {
        case 16:
                if (stereo)
                        return AL_FORMAT_STEREO16;
                else
                        return AL_FORMAT_MONO16;
        case 8:
                if (stereo)
                        return AL_FORMAT_STEREO8;
                else
                        return AL_FORMAT_MONO8;
        default:
                return -1;
        }
}

unsigned int AudioResource(AudioSystem * sys, unsigned int audioId, const GameResource * resource)
{
    unsigned int id =  AudioCreateBuffer(resource); 
    return id;
}


void AudioHandleError()
{
    ALCenum error;

    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        printf("OpenAL Error happened");
    }
}