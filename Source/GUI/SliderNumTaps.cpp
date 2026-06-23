#pragma once
#include "SliderNumTaps.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
SliderNumTaps::SliderNumTaps(MultiTapDelay& d) //Tiene una referencia del MultiTap, que se la pasa PluginEditor al cual se la pasa PluginProcessor
    : delay(d) //Lista de inicialización
{
    addAndMakeVisible(sliderNumTaps);

    sliderNumTaps.setRange(1,50,1);
    sliderNumTaps.setSliderStyle(juce::Slider::IncDecButtons);

    sliderNumTaps.setTextBoxStyle(
    juce::Slider::TextBoxLeft,
    false,
    50,
    20);
    
    sliderNumTaps.setValue(4);
    
    sliderNumTaps.onValueChange = [this]
    {
        delay.setNumTaps(sliderNumTaps.getValue());
        repaint();
    };
}

