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

    //Hace visibles los sliders de control
    addAndMakeVisible(delayMsSlider1);
    addAndMakeVisible(delayMsSlider2); 
    addAndMakeVisible(delayMsSlider3);

    //Setea rangos de trabajo 
    delayMsSlider1.setRange(10.0, 5000, 0.01);
    delayMsSlider2.setRange(10.0, 5000, 0.01);
    delayMsSlider3.setRange(10.0, 5000, 0.01);

     //Define qué pasa cuando se mueve cada slider — actualiza los valores en el Processor
    delayMsSlider1.onValueChange = [this]{
    audioProcessor.delayMs[0] = delayMsSlider1.getValue();
    audioProcessor.updateDelaySamples(); //Llama a la función para actualizar el valor en tiempo real
    };

    delayMsSlider2.onValueChange = [this]{
    audioProcessor.delayMs[1] = delayMsSlider2.getValue();
    audioProcessor.updateDelaySamples();
    };

    delayMsSlider3.onValueChange = [this]{
    audioProcessor.delayMs[2] = delayMsSlider3.getValue();
    audioProcessor.updateDelaySamples();
    };

    //Declara valores iniciales
    delayMsSlider1.setValue(1000.0f);
    delayMsSlider2.setValue(2000.0f);
    delayMsSlider3.setValue(3000.0f);
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

void MiPluginDELAYEditor::resized() 
{
    //Acomodar en pantalla los sliders
    delayMsSlider1.setBounds(20, 20, 200, 40);
    delayMsSlider2.setBounds(20, 80, 200, 40);
    delayMsSlider3.setBounds(20, 140, 200, 40);
}
