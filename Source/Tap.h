#pragma once
#include <juce_audio_processors/juce_audio_processors.h>


// Configura una única línea de retardo 
// Ficha con dos valores
// No procesa, es sólo una estructura simple de datos. AGGREGATE
struct Tap
{
        int delayMs = 0;
        float gain = 0.0f;
};