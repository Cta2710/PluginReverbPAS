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

    tapTable.setModel(this);
    tapTable.getHeader().addColumn("Delay", 1, 80);
    tapTable.getHeader().addColumn("Gain", 2, 80);
    tapTable.getHeader().addColumn("Pan", 3, 80);
    tapTable.getHeader().addColumn("Mute", 4, 60);

    addAndMakeVisible(tapTable);
}

int MiPluginDELAYEditor::getNumRows()
{
    return audioProcessor.getNumTaps();
}

void MiPluginDELAYEditor::paintRowBackground(
    juce::Graphics& g,
    int,
    int,
    int,
    bool rowIsSelected)
{
    g.fillAll(rowIsSelected ? juce::Colours::darkgrey
                            : juce::Colours::black);
}

void MiPluginDELAYEditor::paintCell(
    juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool)
{
    const auto& tap = audioProcessor.getTap(rowNumber);

    juce::String text;

    switch (columnId)
    {
        case 1: text = juce::String(tap.delayMs); break;
        case 2: text = juce::String(tap.gain); break;
        default: break;
    }

    g.setColour(juce::Colours::white);

    g.drawText(text,
               2,
               0,
               width - 4,
               height,
               juce::Justification::centredLeft);
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
{   tapTable.setBounds(getLocalBounds().reduced(10)); }
