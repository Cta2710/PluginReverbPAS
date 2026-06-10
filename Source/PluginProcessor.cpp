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
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

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

readIndex = 0;
writeIndex = 0;

currentSampleRate = getSampleRate(); //Guardo el sample rate para usarlo después

maxDelayTime = 5000.0f; //Delay máximo en segundos
delayBuffSize = currentSampleRate * (maxDelayTime / 1000.0); //Buffer de 5 segundos de duración (en muestras). 
                                       //Permite delayMs MAX de 5000 ms. Aunque el buffer es circular, no podés leer más atrás de lo que tu memoria almacena.
delayBuffer.assign(delayBuffSize, 0.0f);

delayMs[0] = 1000.0f;
delayMs[1] = 2500.0f;
delayMs[2] = 3000.0f;


//ACÁ HAGO MIS TABLAS de retardo, de amplitud máxima, de coeficientes de decaimiento y de frecuencia de corte del filtro
//En ese caso no puedo modificar la reverb mientras está corriendo el audio. Y si lo hago en PB?
}

void MiPluginDELAY::updateDelaySamples()
{
    for (int tap = 0; tap < tapNum; tap++)
    {
        delaySamples[tap] =
            static_cast<int>((delayMs[tap] / 1000.0f) * currentSampleRate);
    }
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

        auto* data = buffer.getWritePointer(0); //Puntero al buffer del DAW para ese canal
        
        for (int n = 0; n < buffer.getNumSamples(); ++n) // para cada sample del bloque
        { 
            float in = data[n]; //Valor del sample actual
            float sum = 0.0f; //Acumulador para el valor del sample retrasado (delay)
        
         // VARIOS PUNTOS DE LECTURA PARA HACER VARIAS REPETICIONES DEL DELAY
            for (int tap = 0; tap < tapNum; tap++) 
            {
                int readIndex = (writeIndex - delaySamples[tap] + delayBuffSize) % delayBuffSize; //Posición de lectura. LEE EL PASADO
                sum += delayBuffer[readIndex]; //Acumula el valor del sample retrasado para todas las repeticiones
            }

         //PARA DELAY CON FEEDBACK delayBuffer[writeIndex] = in + delayed; //Escribo en el buffer circular el sample actual + el retrasado
            delayBuffer[writeIndex] = in; //Escribo en el buffer circular el sample actual. UNA SOLA REPETICIÓN porque no tiene feedback

            data[n] = in + sum; //Escribo en el buffer del DAW el sample actual + el retrasado (el resultado final)

            // UN SÓLO PUNTO DE ESCRITURA
            writeIndex++; //Avanza el índice de escritura 
            // writeIndex %= delayBuffSize; 
            writeIndex = (writeIndex == delayBuffSize) ? 0 : writeIndex;
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

