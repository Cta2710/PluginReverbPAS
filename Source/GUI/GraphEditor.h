// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/MultiTapDelay/MultiTapDelay.h"

//La función de esta clase es crear una gráfico especializado en representar un único parámetro del MultiTapDelay

class GraphEditor : public juce::Component

{
public:

    float maxValue = 0.0f;

    enum class Parameter //Creo variables del tipo Parameter
    {
        Delay, //Guardamos los nombres
        Gain
    };

    GraphEditor(MultiTapDelay& d, Parameter parameter);

    void paint (juce::Graphics& g) override;

    void mouseDown (const juce::MouseEvent&) override;

    void mouseDrag (const juce::MouseEvent&) override;

    void mouseUp (const juce::MouseEvent&) override;

private:
    
    MultiTapDelay& delay; //Referencia del delay

    Parameter parameter;

    int selectedPoint = -1;

    float graphMargin = 20.0f;

    // Conversión entre modelo y gráfico
    float getValueForTap(int tap) const;
    void setValueForTap(int tap, float value);

    // Conversión de coordenadas
    juce::Point<float> getPointPosition(int tap) const;

    float valueToY(float value) const;
    float yToValue(float y) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphEditor)
};