#pragma once
#include "TableEditor.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
TableEditor::TableEditor(MultiTapDelay& d) //Tiene una referencia del MultiTap, que se la pasa PluginEditor al cual se la pasa PluginProcessor
    : delay(d)
{
    tapTable.setModel(this);
    tapTable.getHeader().addColumn("Delay", 1, 80);
    tapTable.getHeader().addColumn("Gain", 2, 80);

    addAndMakeVisible(tapTable);
    addAndMakeVisible(sliderNumTaps);

    sliderNumTaps.setRange(1,50,1);
    sliderNumTaps.setSliderStyle(juce::Slider::IncDecButtons);

    sliderNumTaps.setTextBoxStyle(
    juce::Slider::TextBoxLeft,
    false,
    50,
    20);
    
    sliderNumTaps.setValue(4);
    
    sliderNumTaps.onValueChange = [this]
    {
        delay.setNumTaps(sliderNumTaps.getValue());
        tapTable.updateContent();
        repaint();
    };
}

int TableEditor::getNumRows()
{
    return delay.getNumTaps();
}

void TableEditor::paintRowBackground(
    juce::Graphics& g,
    int,
    int,
    int,
    bool rowIsSelected)
{
    g.fillAll(rowIsSelected ? juce::Colours::darkgrey
                            : juce::Colours::black);
}

void TableEditor::paintCell(
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

juce::Component* TableEditor::refreshComponentForCell(
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
            repaint();
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

void TableEditor::resized()
{
    auto area = getLocalBounds();

    sliderNumTaps.setBounds(area.removeFromBottom(50));

    tapTable.setBounds(area);
}

