#pragma once  // que este archivo se incluya una sola vez
#include "Tap.h"
#include <vector>

// Esta clase es un efecto de delay del tipo multi tap
// Gestiona el buffer circular y hereda de la clase Tap para procesar el audio.

class MultiTapDelay
{
public:

//--------------------Constructor y Destructor
    MultiTapDelay();
    
//--------------------Funciones
    // Configurado del delay
    void prepare(double sampleRate, float maxDelayMs);
    void reset();
    // Procesado del delay
    float process(float input);

     // Gestión de taps
    void setTap(int index, const Tap& tap);
    // void addTap();
    // void addMultipleTaps(int numTapsAdd);
    void setNumTaps(int numTapsSet);
    
    // Para configurar los taps
    
    int getNumTaps() const;
    Tap& getTap(int index);
    const Tap& getTap(int index) const;
    
private: 

    //--------------------Variables
    // De configuración
    float currentSampleRate;
    float maxDelayTime;
    int delayBuffSize;

    // Para construir el buffer circular    
    std::vector<float> delayBuffer; 
    // Para usar el buffer circular    
    int writeIndex = 0;

    //--------------------Lista de taps
    std::vector<Tap> taps;
};