// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "../GUI/TableEditor.h"
#include "../GUI/GraphEditor.h"

//La función de esta clase es construir la interfaz y sus componentes

class MiPluginDELAYEditor : public juce::AudioProcessorEditor,
                            public juce::Timer

{
public:
    MiPluginDELAYEditor(MiPluginDELAY&); //Tiene en el constructor una referencia de la clase de PluginProcessor

    void paint(juce::Graphics&) override;     // dibuja todo
    void resized() override;                   // cuando cambia el tamaño
    void timerCallback() override;

private:

    //REFERENCIAS
    MiPluginDELAY& audioProcessor;     // Referencia directa al PluginProcessor
   
    TableEditor tapTable;                 //Objeto miembro: PluginEditor es dueño de un TableEditor

    GraphEditor delayGraph;            //Objetos miembro: PluginEditor es dueño de los GraphEditor

    GraphEditor gainGraph;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiPluginDELAYEditor)
};
