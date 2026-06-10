// ============================================================
// PluginEditor.cpp — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// La CARA del plugin — dibuja el espectro en pantalla.
//
// Cómo funciona:
//   1. startTimerHz(30) → arranca un reloj que suena 30 veces por segundo
//   2. Cada vez que suena → timerCallback() → repaint()
//   3. repaint() dispara paint() → dibuja todo de nuevo
//
// paint() lee spectrum[] del Processor y dibuja una barra
// de color por cada bin de frecuencia.
// ============================================================

#include "PluginEditor.h"
#include <cmath>

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
MiPluginDELAYEditor::MiPluginDELAYEditor(
    MiPluginDELAY& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(1000, 400);   // ventana de 1000 x 400 píxeles
    startTimerHz(30);     // redibujar 30 veces por segundo

    for (int tap = 0; tap < audioProcessor.tapNum; ++tap)
     {
        addAndMakeVisible(delayMsSlider[tap]); //Hace visibles los sliders de control
        delayMsSlider[tap].setRange(10.0, 5000, 0.01); //Setea rangos de trabajo de 10 ms a 5000 ms con pasos de 0.01 ms
        delayMsSlider[tap].setValue(1000.0f * (tap + 1)); //Declara valores iniciales de mil en mil
        delayMsSlider[tap].onValueChange = [this, tap]{
            audioProcessor.delayMs[tap] = delayMsSlider[tap].getValue(); //Actualiza el valor de delayMs en el Processor
            audioProcessor.updateDelaySamples(); //Llama a la función para actualizar el valor en tiempo real
        };
    }
}

// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
MiPluginDELAYEditor::~MiPluginDELAYEditor()
{
    stopTimer();          // parar el reloj al cerrar
}

// ─── TIMER ────────────────────────────────────────────────────────────────────
void MiPluginDELAYEditor::timerCallback()
{
    repaint();            // "redibujá la pantalla" → dispara paint()
}

// ─── PAINT — DIBUJA TODO ──────────────────────────────────────────────────────
void MiPluginDELAYEditor::paint(juce::Graphics& g)
{
  //Dibuja texto y fondo
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Delay Plugin", getLocalBounds(), juce::Justification::centredTop);

}

void MiPluginDELAYEditor::resized() {}
