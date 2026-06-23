#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/MultiTapDelay/MultiTapDelay.h"

//La función de esta clase es crear una tabla de ms y gain que modifique al MultiTapDelay

class SliderNumTaps : public juce::Component

{
public:
    SliderNumTaps(MultiTapDelay& d);
    void resized() override;


private:

    juce::Slider sliderNumTaps; //Declaro una instancia de la clase Slider
    
    MultiTapDelay& delay; //Referencia del delay

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderNumTaps)
};