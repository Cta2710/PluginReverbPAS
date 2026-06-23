// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

//La función de esta clase es comunicarse con el DAW

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
// Se ejecuta UNA sola vez cuando Reaper carga el plugin.
// Acá reservamos memoria para todos los buffers.
// IMPORTANTE: nunca reservar memoria en processBlock — causa glitches.
// Es boilerplate — siempre es igual.
MiPluginDELAY::MiPluginDELAY()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
// Destructor vacío. Los vectores se liberan solos cuando el plugin se cierra.
MiPluginDELAY::~MiPluginDELAY() {}

//GETTER: Función para poder referenciar al objeto miembro
MultiTapDelay& MiPluginDELAY::getDelay()
{
    return delay;
}

// ─── PREPARE TO PLAY ──────────────────────────────────────────────────────────
void MiPluginDELAY::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    delay.prepare(sampleRate, 5000.0f);
}

// ─── RELEASE RESOURCES ────────────────────────────────────────────────────────
void MiPluginDELAY::releaseResources() {}
// se llama al cerrar — nada que liberar


// ─── PROCESS BLOCK — EL CORAZÓN DEL PLUGIN ───────────────────────────────────
void MiPluginDELAY::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer&)
{
    for (int ch = 0; ch< buffer.getNumChannels(); ch++)
    {
        auto* channelData = buffer.getWritePointer(ch); //Puntero al buffer del DAW del canal actual.
        
        for (int samp = 0; samp < buffer.getNumSamples(); ++samp) //Recorro cada sample del bloque del canal actual.
        { 
            float input = channelData[samp]; //Leo el sample de entrada
            float outputMultiTapDelay = delay.process(input); //Proceso
            channelData[samp] = outputMultiTapDelay; //Escribo el audio procesado en el buffer
            //Era lo mismo hacer channelData[samp] = delay.process(channelData[samp]);
        }
    }
}

// ─── CREAR EDITOR ─────────────────────────────────────────────────────────────
// Crea la ventana del plugin (el Editor). Le pasa *this — 
// una referencia al Processor — para que el Editor pueda leer.
juce::AudioProcessorEditor* MiPluginDELAY::createEditor()
{
    return new MiPluginDELAYEditor(*this);
    // *this = le pasa una referencia al Processor
    // el Editor la guarda para leer
}

// ─── PUNTO DE ENTRADA ─────────────────────────────────────────────────────────
// Punto de entrada. JUCE llama esta función para crear el plugin. 
// Siempre es igual, no se toca.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MiPluginDELAY();
    // JUCE llama esto para crear el plugin — siempre es igual
}

