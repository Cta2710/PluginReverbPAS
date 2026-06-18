#include "Taps.h"

void Tap :: setTap(float newDelayMs, float sampleRate) //Función que inicializa valores del tap
{
    delMs = newDelayMs; //Actualizo el tiempo de retardo en milisegundos.
    delSamp=
            (delMs / 1000.0f) * sampleRate; //Calculo el retraso en muestras.
}

int Tap:: getTap() //Función que devuelve valores del tap
{
    return delSamp;
}
