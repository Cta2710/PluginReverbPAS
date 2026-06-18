// ============================================================
// PluginProcessor.cpp — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// IMPLEMENTACIÓN del Processor.
//
// Todo el DSP vive en processBlock().
// El resto es setup y boilerplate.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Tap.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
// Se ejecuta UNA sola vez cuando Reaper carga el plugin.
// Acá reservamos memoria para todos los buffers.
// IMPORTANTE: nunca reservar memoria en processBlock — causa glitches.
// Es boilerplate — siempre es igual.
MiPluginDELAY::MiPluginDELAY()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)) 
 {
    //Agrego elementos al vector Taps
    //Lista de inicialización con push_back, miembros en orden de declaración de la clase. Crea el vector con valores por defecto
    taps.push_back({ 1000, 0.8f });
    taps.push_back({ 2000, 0.6f });
    taps.push_back({ 3000, 0.3f });
 }

// ─── FUNCIONES PARA EDITOR───────────────────────────────────────────────────────    
int MiPluginDELAY::getNumTaps() const
{   return static_cast<int>(taps.size()); }                 //Retorna el tamaño del vector de taps

const Tap& MiPluginDELAY::getTap(int index) const
{   return taps[index]; }                                   //Retorna el índice 

void MiPluginDELAY::setTapDelay(int index, float delayMs)    //Modifica el retardo de cada tap según GUI
{   taps[index].delayMs = delayMs; }

void MiPluginDELAY::setTapGain(int index, float gain)        //Modifica el nivel de cada tap según GUI
{    taps[index].gain = gain; }

// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
// Destructor vacío. Los vectores se liberan solos cuando el plugin se cierra.
MiPluginDELAY::~MiPluginDELAY() {}


/* Dos funciones que JUCE obliga a implementar. 
prepareToPlay se llama cuando Reaper está por empezar a mandar audio 
(te dice el sample rate y el tamaño de bloque). 
releaseResources cuando para.
En este plugin no necesitamos hacer nada en ninguna de las dos.*/

// ─── PREPARE TO PLAY ──────────────────────────────────────────────────────────
void MiPluginDELAY::prepareToPlay(double, int) 
{
    //-------Setup de variables en tiempo de compilación

    readIndex = 0; //Inicializo el índice de lectura del buffer circular en 0.
    writeIndex = 0; //Inicializo el índice de escritura del buffer circular en 0.

    currentSampleRate = getSampleRate(); //Obtengo el sample rate del DAW.

    maxDelayTime = 5000.0f; //Delay máximo en segundos - define el tamaño del buffer circular.
    delayBuffSize = currentSampleRate * (maxDelayTime / 1000.0); //Transformo el delay máximo de ms a samples para definir el tamaño del buffer circular.
                                       //Permite delayMs MAX de 5000 ms. Aunque el buffer es circular, no se puede leer más atrás de lo que la memoria almacena.
    delayBuffer.assign(delayBuffSize, 0.0f); //Reservo memoria para el buffer circular, lo lleno de ceros.
}

// ─── RELEASE RESOURCES ────────────────────────────────────────────────────────
void MiPluginDELAY::releaseResources() {}
// se llama al cerrar — nada que liberar


// ─── PROCESS BLOCK — EL CORAZÓN DEL PLUGIN ───────────────────────────────────
void MiPluginDELAY::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer&)
{
    auto* data = buffer.getWritePointer(0); //Puntero al buffer del DAW para el primer canal (mono).
        
    for (int n = 0; n < buffer.getNumSamples(); ++n) //Recorro cada sample del bloque.
    { 
        float in = data[n]; //Leo el sample de entrada
        float sum = 0.0f; //Inicializo el acumulador.

        for (const auto& tap : taps) //Recorro vector de taps
        {
            delaySamples =  ((tap.delayMs / 1000.0f) * currentSampleRate); //Transformo el delay de ms a samples

            int readIndex = (writeIndex - delaySamples + delayBuffSize) % delayBuffSize;   //Calculo donde leer
                                                                                                    //Al sample actual le resto el delay en samples para leer el valor retrasado. Sumo delayBuffSize para evitar índices negativos, y uso módulo para que el buffer sea circular.   
            sum += tap.gain * delayBuffer[readIndex]; //Acumula el valor del sample retrasado ponderado por la ganancia

        }
            
        delayBuffer[writeIndex] = in; //Escribo en el buffer circular el sample actual.

        data[n] = in + sum; //Genero la salida
                            // Escribo en el buffer del DAW el sample actual + los retrasados (OUTPUT).

        writeIndex++; //Avanza el índice de escritura 
        writeIndex = (writeIndex == delayBuffSize) ? 0 : writeIndex; //Si el índice de escritura llega al final del buffer, vuelve al principio (circular).
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

