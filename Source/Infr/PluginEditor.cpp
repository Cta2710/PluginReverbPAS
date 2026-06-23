// ============================================================

#include "PluginEditor.h"
#include <cmath>

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
MiPluginDELAYEditor::MiPluginDELAYEditor(
    MiPluginDELAY& p)
    : AudioProcessorEditor(&p)
                            , audioProcessor(p)
                            , tapTable(p.getDelay())
                            //p es una referencia al PluginProcessor. 
                            //p.getDelay() devuelve MultiTapDelay& y se pasa al constructor
                            // table(...) inicializa el miembro con la referencia.
                            ,delayGraph(p.getDelay(), GraphEditor::Parameter::Delay)
                            ,gainGraph(p.getDelay(), GraphEditor::Parameter::Gain)
                            ,sliderNumTaps(p.getDelay())

{
    addAndMakeVisible(tapTable);
    addAndMakeVisible(delayGraph);
    addAndMakeVisible(gainGraph);
    addAndMakeVisible(sliderNumTaps);
    setSize(400,500);
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
    auto area = getLocalBounds(); //Area total, depende del tamaño de ventana
    area.removeFromTop(50);
    sliderNumTaps.setBounds(area.removeFromTop(50));

    tapTable.setBounds(area.removeFromLeft(200));
    delayGraph.setBounds(area);
     

    // auto graphArea = area;
    // delayGraph.setBounds(graphArea.removeFromTop(graphArea.getHeight() / 2));
    // gainGraph.setBounds(graphArea);

//    auto area = getLocalBounds();

    




}
