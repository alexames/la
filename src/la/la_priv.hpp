#ifndef LA_PRIV_H_
#define LA_PRIV_H_

#include <vector>
#include "MemoryPool.hpp"

struct la_State
{
    PaStream* stream;

    float tunedTo;
    unsigned int sampleRate;

    // The currently playing sounds
    std::vector<la_Sound*> sounds;

    std::vector<la_Event*> events;
    std::vector<la_Event*> immediateEvents;
    unsigned int nextEventIndex;

    float beat;
    float tempo;

    MemoryPool<la_Sound, 128> pool;

    bool playing;
};

extern la_State globalState;

int la_CompileSounds(const void *inputBuffer, void* outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void *userdata);

#endif // LA_PRIV_H_
