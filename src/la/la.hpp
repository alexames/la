#ifndef LA_H_
#define LA_H_

#include <math.h>
#include "portaudio.h"

#ifndef paCheck
#define paCheck(x) \
    do { PaError err = (x); if (err != paNoError) { printf("(line %d) PortAudio error: %s\n", __LINE__, Pa_GetErrorText(err)); } }  while (0)
#endif

#ifndef TRACE
#define TRACE() \
    printf("%s:%d:%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

typedef enum
{
    LA_SQUARE,
    LA_SINE,
    LA_TRIANGLE,
    LA_SAWTOOTH,
    LA_NOISE,

    LA_SAMPLE
} la_WaveType;

typedef struct la_Event
{
    float tone;
    float beat;
    float volume;
    float length;
    la_WaveType wavetype;

    float vibrato;

} la_Event;

typedef struct la_Sound
{
    float theta;
    float tone;
    float beatsleft;
    float volume;
    float previousamplitude;
    char readytodie;
    la_WaveType wavetype;
} la_Sound;

void la_Init();
void la_Deinit();
void la_Play();
void la_Pause();
void la_Stop();
void la_SetTempo(float tempo);

float la_GetBeat();

void la_AddEvent(la_Event* event);

#endif // LA_H_

