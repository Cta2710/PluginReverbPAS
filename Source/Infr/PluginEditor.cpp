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

{
    addAndMakeVisible(tapTable);
    addAndMakeVisible(delayGraph);
    addAndMakeVisible(gainGraph);
    setSize(800,500);
    startTimerHz(30);
}

void MiPluginDELAYEditor::timerCallback() 
{
    tapTable.refreshTable();
    delayGraph.repaint();
    gainGraph.repaint();
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
    area.removeFromTop(40);

    tapTable.setBounds(area.removeFromLeft(200));
    
    auto altoDeCadaGrafico = area.getHeight() / 2;
    delayGraph.setBounds(area.removeFromTop(altoDeCadaGrafico));
    gainGraph.setBounds(area);
     

    // auto graphArea = area;
    // delayGraph.setBounds(graphArea.removeFromTop(graphArea.getHeight() / 2));
    // gainGraph.setBounds(graphArea);

//    auto area = getLocalBounds();

    




}
