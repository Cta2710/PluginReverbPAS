// ============================================================

#pragma once  // que este archivo se incluya una sola vez
#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/MultiTapDelay/MultiTapDelay.h" //sube una carpeta y busca
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

    MultiTapDelay& getDelay(); //Referencia al delay

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiPluginDELAY);

    MultiTapDelay delay; //Objeto miembro
};