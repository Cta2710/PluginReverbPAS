// ============================================================
// PluginProcessor.h — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// El Processor es el CEREBRO del plugin.
// Recibe audio del DAW, lo procesa, y guarda los resultados.
// El Editor lee esos resultados para dibujar en pantalla.
//
// Este archivo es la DECLARACIÓN: dice qué funciones y
// variables existen, pero no cómo funcionan.
// La implementación está en PluginProcessor.cpp.
// ============================================================

#pragma once  // que este archivo se incluya una sola vez

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>  // acá vive juce::dsp::FFT
#include "Tap.h"
#include <vector>

class MiPluginDELAY : public juce::AudioProcessor

{
public:
    MiPluginDELAY();
    ~MiPluginDELAY() override;

    // --- las 3 funciones del ciclo de vida ---
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    // processBlock = el corazón. El DAW la llama ~86 veces por segundo.

    // --- crear la ventana del plugin ---
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    // --- boilerplate (JUCE lo necesita, no lo tocamos) ---
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    
    // void updateDelaySamples();    
    float currentSampleRate;
    float maxDelayTime;
    int delayBuffSize;
    std::vector<float> delayBuffer; //Buffer circular para el delay
    int delaySamples;

    int writeIndex = 0;
    int readIndex = 0;
   
// static constexpr int tapNum = 3; //Tiene que ser una constante para poder usarla en la declaración de los arrays
    // int delaySamples[tapNum];
    // float delayMs[tapNum]; 

//Para la clase Tap
    int getNumTaps() const;
    const Tap& getTap(int index) const;
    void setTapDelay(int index, float delayMs);
    void setTapGain(int index, float gain);


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiPluginDELAY);
    std::vector<Tap> taps;
};