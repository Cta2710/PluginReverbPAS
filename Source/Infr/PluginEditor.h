// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "../GUI/TableEditor.h"

class MiPluginDELAYEditor : public juce::AudioProcessorEditor

{
public:
    MiPluginDELAYEditor(MiPluginDELAY&); //Tiene en el constructor una referencia de la clase de PluginProcessor

    void paint(juce::Graphics&) override;     // dibuja todo
    void resized() override;                   // cuando cambia el tamaño

private:

    //REFERENCIAS
    MiPluginDELAY& audioProcessor;      // Referencia directa al PluginProcessor
    //MultiTapDelay& delay;               //Referencia al efecto que se la pasa PluginProcessor
    TableEditor table;                 //Objeto miembro: PluginEditor es dueño del TableEditor

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiPluginDELAYEditor)
};
