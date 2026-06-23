// ============================================================

#include "PluginEditor.h"
#include <cmath>

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
MiPluginDELAYEditor::MiPluginDELAYEditor(
    MiPluginDELAY& p)
    : AudioProcessorEditor(&p)
                            , audioProcessor(p)
                            , table(p.getDelay()) 
                            //p es una referencia al PluginProcessor. 
                            //p.getDelay() devuelve MultiTapDelay& y se pasa al constructor
                            // table(...) inicializa el miembro con la referencia.
{
    addAndMakeVisible(table);
    setSize(500,400);
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
    table.setBounds(getLocalBounds());
}
