#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include "la.hpp"
#include "la_priv.hpp"
#include "MemoryPool.hpp"

#ifndef PI
#define PI 3.1415926535
#endif

la_State globalState;

float la_ToneFrequency(double tone)
{
    return globalState.tunedTo * pow(2, tone/12);
}

void la_TickEvents(float dt)
{
    globalState.beat += globalState.tempo * dt / 60;
    la_Event* event;
    if (globalState.nextEventIndex < globalState.events.size())
    {
        event = globalState.events[globalState.nextEventIndex];
        while (globalState.beat >= event->beat)
        {
            la_Sound* sound = globalState.pool.alloc();
            if (sound)
            {
                sound->beatsleft = event->length;
                sound->volume = event->volume;
                sound->wavetype = event->wavetype;
                sound->tone = event->tone;
                sound->previousamplitude = 0.f;
                globalState.sounds.push_back(sound);
            }
            globalState.nextEventIndex++;
            if (globalState.nextEventIndex < globalState.events.size())
            {
                event = globalState.events[globalState.nextEventIndex];
            }
            else
            {
                break;
            }
        }
    }
    // Yes, this leaks memory
    while (globalState.immediateEvents.size())
    {
        event = globalState.immediateEvents.back();
        // Refactor
        la_Sound* sound = globalState.pool.alloc();
        if (sound)
        {
            sound->beatsleft = event->length;
            sound->volume = event->volume;
            sound->wavetype = event->wavetype;
            sound->tone = event->tone;
            sound->previousamplitude = 0.f;
            globalState.sounds.push_back(sound);
        }
    }
}

void la_TickSounds(float dt)
{
    int i;
    for (i = globalState.sounds.size()-1; i >= 0 ; i--)
    {
        la_Sound* sound = globalState.sounds[i];
        float theta = sound->theta;
        sound->theta += dt * la_ToneFrequency(sound->tone);
        if (sound->readytodie && sound->beatsleft < 0)
        {
            globalState.pool.free(globalState.sounds[i]);
            globalState.sounds[i] = globalState.sounds.back();
            globalState.sounds.pop_back();
        }
        else
        {
            sound->beatsleft -= (globalState.tempo * dt / 60);
            if (sound->theta >= 1.0)
                sound->theta -= 1.0;
        }
    }
}

void la_MixSounds(float* left, float* right)
{
    int i;

    *left = 0;
    *right = 0;

    for (i = globalState.sounds.size()-1; i >= 0 ; i--)
    {
        la_Sound* sound = globalState.sounds[i];
        float amplitude;
        switch (sound->wavetype)
        {
            case LA_SQUARE:
                amplitude = sound->theta > .5 ? 1.0 : -1.0;
                break;
            case LA_SINE:
                amplitude = sin(2 * PI * sound->theta);
                break;
            case LA_TRIANGLE:
                amplitude = sound->theta > .5 ? 4 * sound->theta - 3 : -4 * sound->theta + 1;
                break;
            case LA_SAWTOOTH:
                amplitude = 2 * sound->theta - 1.0;
                break;
            case LA_NOISE:
                amplitude = ((float)rand() / RAND_MAX);
                break;
            default:
                ;
        }
        sound->readytodie = (sound->previousamplitude * amplitude <= 0.f);
        sound->previousamplitude = amplitude;
        *left += amplitude * sound->volume;
        *right += amplitude * sound->volume;
    }
}

int la_CompileSounds(const void *inputBuffer, void* outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userdata)
{
    float* out = (float*)outputBuffer;
    unsigned int frame;
    float dt = 1.0/globalState.sampleRate;
    for(frame = 0; frame<framesPerBuffer; frame++)
    {
        float* left = out++;
        float* right = out++;
        if (globalState.playing)
        {
            la_TickEvents(dt);
        }
        la_TickSounds(dt);
        la_MixSounds(left, right);
    }
    return 0;
}

