#pragma once
#include <juce_audio_processors/juce_audio_processors.h>


// Clase que configura una única línea de retardo
class Tap
{
    public:
        void setTap(float newDelayMs, float sampleRate);
        int getTap() const;

    private: 
        float delMs; 
        int delSamp;
        float sRate;
        void updateDelaySamples();

};