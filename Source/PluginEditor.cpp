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
    // addAndMakeVisible(delayMsSlider);
 //   addAndMakeVisible(retardoSlider); 
    //addAndMakeVisible(numTapsSlider);

    //Setea rangos de trabajo 
    // delayMsSlider.setRange(0.0, audioProcessor.maxDelayTime, 0.01);
   // retardoSlider.setRange(0.0, 2000.0, 1.0);
  //  numTapsSlider.setRange(1, 10, 1);

  //Define qué pasa cuando se mueve cada slider — actualiza los valores en el Processor
  // delayMsSlider.onValueChange = [this]{
    // audioProcessor.delayMs = delayMsSlider.getValue();
};

    //Declara valores iniciales
    // delayMsSlider.setValue(0.5f);
  //  retardoSlider.setValue(100.0f);
  //  numTapsSlider.setValue(5.0f);


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

void MiPluginDELAYEditor::resized() {
  //Dibuja los sliders de control
    // delayMsSlider.setBounds(20, 20, 200, 40);
  //  retardoSlider.setBounds(10, 120, 20, 50);
   // numTapsSlider.setBounds(margin_left, margin_top + 60, plot_width, 20);
}
