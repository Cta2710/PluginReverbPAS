//--------------------Clase Tap: funciona como almacenamiento de datos de repeticiones individuales
#pragma once 
#include  "../FiltroPasaBajos.h" //sube una carpeta y busca

    struct Tap
    {
        //Variables generales
        int delayMs = 0;
        float gain = 0.0f;
        bool enabled = true;
        
        //Filtro pasa-bajos (variable para frecuencia de corte y declaración del objeto miembro)
        float cutOffFreq = 20000.0f;
        FiltroPasaBajos lowpass;
    };
