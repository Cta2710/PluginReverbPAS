#include "MultiTapDelay.h"

MultiTapDelay::MultiTapDelay()
{
    //Agrego elementos al vector Taps
    //Lista de inicialización con push_back, miembros en orden de declaración de la clase. Crea el vector con valores por defecto
    taps.push_back({ 1000, 0.9f });
    taps.push_back({ 2000, 0.7f });
    taps.push_back({ 3000, 0.5f });
    taps.push_back({ 4000, 0.3f});
}

// ─── SETEO DE TAPS ───────────────────────────────────────────────────────────────
void MultiTapDelay::addTap()
{
    taps.push_back({ 0000, 0.0f });
}

int MultiTapDelay::getNumTaps() const
{
    return static_cast<int>(taps.size()); 
}                 //Retorna el tamaño del vector de taps

Tap& MultiTapDelay::getTap(int index)
{
    return taps[index];
}

const Tap& MultiTapDelay::getTap(int index) const
{   
    return taps[index]; 
}                                   //Retorna valores de un tap específico

void MultiTapDelay::setTap(int index, const Tap& tap)    //Modifica valores de un tap específico
{   
    taps[index] = tap; 
}

// ─── PREPARE───────────────────────────────────────────────────────────────
void MultiTapDelay::prepare(double sampleRate, float maxDelayMs)
{ 
    currentSampleRate = sampleRate; //Obtengo la frecuencia de muestreo del DAW.
    maxDelayTime = maxDelayMs; //Delay máximo en segundos - define el tamaño del buffer circular.
    
    delayBuffSize = static_cast<int>(currentSampleRate * maxDelayTime / 1000.0); //Transformo el delay máximo de ms a samples para definir el tamaño del buffer circular.
                                       //Permite delayMs MAX de 5000 ms. Aunque el buffer es circular, no se puede leer más atrás de lo que la memoria almacena.
    delayBuffer.assign(delayBuffSize, 0.0f); //Reservo memoria para el buffer circular, lo lleno de ceros.
    
    writeIndex = 0; //Inicializo el índice de escritura del buffer circular en 0.

}

// ─── PROCESAMIENTO ───────────────────────────────────────────────────────────────
float MultiTapDelay::process(float input) //Se recibe del Daw el sample actual y se empieza a guardar en el buffer circular y a procesar el audio.
{
    float in = input; //Agarro la muestra que llega del ProcessBlock    
    float sum = 0.0f; //Empiezo con una suma vacía.

    for (const auto& tap : taps) //Recorro vector de taps
    {
        if (!tap.enabled)
        continue; //Se fija si el tap actual está activado.

        //Para cada tap busco la muestra retrasada
        int delaySamples =  ((tap.delayMs / 1000.0f) * currentSampleRate); //Transformo el delay de ms a samples
        int readIndex = (writeIndex - delaySamples + delayBuffSize) % delayBuffSize;   //Calculo donde leer
            //Al sample actual le resto el delay en samples para leer el valor retrasado. Sumo delayBuffSize para evitar índices negativos, y uso módulo para que el buffer sea circular.       
        float delayedSample = delayBuffer[readIndex];
        //Lo multiplico por su ganancia y lo agrego a la suma
        sum += tap.gain * delayedSample; //Acumula el valor del sample retrasado ponderado por la ganancia
    }
    
    //Guardo la entrada en el buffer
    delayBuffer[writeIndex] = in; //Escribo en el buffer circular el sample actual.
    //Calculo la salida
    float output = in + sum; //Genero la salida para escribir en el buffer del DAW el sample actual + los retrasados (OUTPUT).

    //Avanzo el buffer
    writeIndex++; //Avanza el índice de escritura 
    writeIndex = (writeIndex == delayBuffSize) ? 0 : writeIndex; //Si el índice de escritura llega al final del buffer, vuelve al principio (circular).

    //Devuelvo la salida
    return output;
}


