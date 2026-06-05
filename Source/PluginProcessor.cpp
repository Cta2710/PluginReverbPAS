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

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
// Se ejecuta UNA sola vez cuando Reaper carga el plugin.
// Acá reservamos memoria para todos los buffers.
// IMPORTANTE: nunca reservar memoria en processBlock — causa glitches.
// Es boilerplate — siempre es igual.
MiPluginDELAY::MiPluginDELAY()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
        
            //double currentSampleRate = sampleRate;
        }

/* Fin del constructor. Todo lo que reservamos acá queda disponible para siempre. 
Por eso lo hacemos acá y no en processBlock — si reservás memoria en processBlock, 
el audio se corta.
 */

 
// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
// Destructor vacío. Los vectores se liberan solos cuando el plugin se cierra.
MiPluginDELAY::~MiPluginDELAY() {}


/* Dos funciones que JUCE obliga a implementar. 
prepareToPlay se llama cuando Reaper está por empezar a mandar audio 
(te dice el sample rate y el tamaño de bloque). 
releaseResources cuando para.
En este plugin no necesitamos hacer nada en ninguna de las dos.*/

// ─── PREPARE TO PLAY ──────────────────────────────────────────────────────────
void MiPluginDELAY::prepareToPlay(double, int) {
// el DAW avisa a qué sample rate va a correr

currentSampleRate = getSampleRate(); //Guardo el sample rate para usarlo después

maxDelayTime = 5000.0f; //Delay máximo en segundos
delayBuffSize = currentSampleRate * (maxDelayTime / 1000.0); //Buffer de 5 segundos de duración (en muestras). 
                                       //Permite delayMs MAX de 5000 ms. Aunque el buffer es circular, no podés leer más atrás de lo que tu memoria almacena.
delayBuffer.assign(delayBuffSize, 0.0f);

delayMs = 500.0f; //Delay en milisegundos
writeIndex = 0;
readIndex = 0;

//ACÁ HAGO MIS TABLAS de retardo, de amplitud máxima, de coeficientes de decaimiento y de frecuencia de corte del filtro
//En ese caso no puedo modificar la reverb mientras está corriendo el audio. Y si lo hago en PB?
}

// ─── RELEASE RESOURCES ────────────────────────────────────────────────────────
void MiPluginDELAY::releaseResources() {}
// se llama al cerrar — nada que liberar


// ─── PROCESS BLOCK — EL CORAZÓN DEL PLUGIN ───────────────────────────────────
// El DAW llama a esta función ~86 veces por segundo.
// Cada vez manda un bloque de ~512 samples de audio.
//                                                        
// ║  Los pasos son:                                          ║
// ║  1. Leer samples del DAW                                 ║
// ║  2. Acumular hasta tener buffer_size                     ║
// ║  3. Copiar al buffer de JUCE                             ║
// ║  4. Procesar                                             ║
// ║  5. Resetear y empezar de nuevo                          ║
// ╚══════════════════════════════════════════════════════════╝
//
void MiPluginDELAY::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer&)
{
    auto* data = buffer.getWritePointer(0); //Puntero al buffer del DAW

    delaySamples = (int)((delayMs / 1000.0) * currentSampleRate); //Delay en ms convertido a muestras

    for (int n = 0; n < buffer.getNumSamples(); ++n) // para cada sample del bloque
    { 
    readIndex = (writeIndex - delaySamples + delayBuffSize) % delayBuffSize; //Posición de lectura. LEE EL PASADO
    float delayed = delayBuffer[readIndex]; //Obtiene el valor del sample retrasado
    float in = data[n]; //Valor del sample actual

   //PARA DELAY CON FEEDBACK delayBuffer[writeIndex] = in + delayed; //Escribo en el buffer circular el sample actual + el retrasado
    delayBuffer[writeIndex] = in; //Escribo en el buffer circular el sample actual. UNA SOLA REPETICIÓN porque no tiene feedback

    data[n] = in + delayed; //Escribo en el buffer del DAW el sample actual + el retrasado (el resultado final)

    writeIndex++; //Avanza el índice de escritura
    writeIndex = (writeIndex == delayBuffer.size()) ? 0 : writeIndex; //Operador ternario (if implícito)
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

