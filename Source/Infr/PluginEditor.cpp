// ============================================================

#include "PluginEditor.h"
#include <cmath>

class EditableLabel : public juce::Label
{
public:
    int row = 0;
    int column = 0;
};

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
MiPluginDELAYEditor::MiPluginDELAYEditor(
    MiPluginDELAY& p)
    : AudioProcessorEditor(&p)
                            , audioProcessor(p)
                            , delay(p.getDelay()) 
                            //p es una referencia al PluginProcessor. 
                            //p.getDelay() devuelve una referencia al MultiTapDelay del processor.
                            // delay(...) inicializa el miembro delay del editor con esa referencia.
{
    setSize(1000, 400);   // ventana de 1000 x 400 píxeles
    startTimerHz(30);     // redibujar 30 veces por segundo

    tapTable.setModel(this);
    tapTable.getHeader().addColumn("Delay", 1, 80);
    tapTable.getHeader().addColumn("Gain", 2, 80);

    addAndMakeVisible(tapTable);
    addAndMakeVisible(addTapButton);
    
    addTapButton.onClick = [this]
    {
        delay.addTap();
        tapTable.updateContent();
    };
}

int MiPluginDELAYEditor::getNumRows()
{
    return delay.getNumTaps();
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
    const auto& tap = delay.getTap(rowNumber);

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

juce::Component* MiPluginDELAYEditor::refreshComponentForCell(
    int rowNumber,
    int columnId,
    bool,
    juce::Component* existingComponentToUpdate)
{
    auto* label = dynamic_cast<EditableLabel*>(existingComponentToUpdate);

    if (label == nullptr)
    {
        label = new EditableLabel();

        label->setEditable(true);
        label->setJustificationType(juce::Justification::centredLeft);

        label->onTextChange = [this, label]
        {
            auto tap = delay.getTap(label->row);

            switch (label->column)
            {
                case 1: // Delay
                    tap.delayMs = label->getText().getIntValue();
                    break;

                case 2: // Gain
                    tap.gain = label->getText().getFloatValue();
                    break;
            }

            delay.setTap(label->row, tap);
        };
    }

    label->row = rowNumber;
    label->column = columnId;

    const auto& tap = delay.getTap(rowNumber);

    switch (columnId)
    {
        case 1: // Delay
            label->setText(juce::String(tap.delayMs),
                           juce::dontSendNotification);
            break;

        case 2: // Gain
            label->setText(juce::String(tap.gain),
                           juce::dontSendNotification);
            break;
    }

    return label;
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
{   addTapButton.setBounds(10, 10, 30, 25);
    
    tapTable.setBounds(10, 50, getWidth() - 20, getHeight() - 60);}
