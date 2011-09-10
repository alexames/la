#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portaudio.h"
#include "la.hpp"
#include "la_priv.hpp"
#include "MemoryPool.hpp"

#ifndef PI
#define PI 3.14159265
#endif

const unsigned int CD_SAMPLE_RATE = 44100;
/* const float MIDDLE_C = 261.626f; */
const float MIDDLE_C = 440.f;

void la_Init()
{
    globalState.stream = NULL;
    globalState.tunedTo = MIDDLE_C;
    globalState.sampleRate = CD_SAMPLE_RATE;
    globalState.nextEventIndex = 0;
    globalState.beat = 0;
    globalState.tempo = 120;
    globalState.playing = 0;

    paCheck(Pa_Initialize());
    paCheck(Pa_OpenDefaultStream(&globalState.stream, 0, 2, paFloat32, globalState.sampleRate, 256, la_CompileSounds, &globalState));
    paCheck(Pa_StartStream(globalState.stream));
}

void la_Deinit()
{
    globalState.playing = 0;
    paCheck(Pa_StopStream(globalState.stream));
    paCheck(Pa_CloseStream(globalState.stream));
    paCheck(Pa_Terminate());
}

void la_Play()
{
    globalState.playing = 1;
}

void la_Pause()
{
    globalState.playing = 0;
}

void la_Toggle()
{
    globalState.playing = !globalState.playing;
}

void la_Stop()
{
    globalState.beat = 0.0f;
    globalState.playing = 0;
}

float la_GetBeat()
{
    return globalState.beat;
}

void la_SetTempo(float tempo)
{
    if (tempo > 0)
        globalState.tempo = tempo;
    else
        globalState.tempo = 120;
}

void la_AddEvent(la_Event* event)
{
    if (event)
    {
        char add = 0;
        la_Event* tail = NULL;
        int i;
        if (event->beat < 0)
        {
            globalState.immediateEvents.push_back(event);
        }
        if (!globalState.events.empty())
            tail = globalState.events.back();

        if (!tail)
        {
            add = 1;
        }
        else
        {
            if (abs(tail->beat - event->beat) < .000001)
                event->beat = tail->beat;

            if (tail->beat <= event->beat)
                add = 1;
        }

        if (add)
            globalState.events.push_back(event);
    }
}
