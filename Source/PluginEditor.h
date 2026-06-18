// ============================================================
// PluginEditor.h — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// El Editor es la CARA del plugin — lo que ves en Reaper.
// Hereda de Timer para redibujar 30 veces por segundo.
// Tiene una referencia al Processor para leer spectrum[].
// El audio NUNCA pasa por acá — solo datos para mostrar.
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class MiPluginDELAYEditor : public juce::AudioProcessorEditor,
                            public juce::Timer,
                            public juce::TableListBoxModel

{
public:
    MiPluginDELAYEditor(MiPluginDELAY&);
    ~MiPluginDELAYEditor() override;

    void paint(juce::Graphics&) override;     // dibuja todo
    void resized() override;                   // cuando cambia el tamaño
    void timerCallback() override;             // se llama 30 veces por segundo

   // TableListBoxModel
    int getNumRows() override;

    void paintRowBackground(juce::Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(juce::Graphics&,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

private:
    MiPluginDELAY& audioProcessor;
    // referencia al Processor

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiPluginDELAYEditor)

    juce::TableListBox tapTable;
};
