// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/MultiTapDelay/MultiTapDelay.h"

//La función de esta clase es crear una tabla de ms y gain que modifique al MultiTapDelay

class TableEditor : public juce::Component,
                            public juce::TableListBoxModel

{
public:
    TableEditor(MultiTapDelay& d);

   // void paint(juce::Graphics&) override;
    void resized() override;
    
   // TableListBoxModel
    int getNumRows() override;

    void paintRowBackground(juce::Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(juce::Graphics&,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    juce::Component* refreshComponentForCell(int rowNumber,
                                         int columnId,
                                         bool isRowSelected,
                                         juce::Component* existingComponentToUpdate) override;

    class EditableLabel : public juce::Label
{
    public:
    int row = 0;
    int column = 0;
};

private:

    juce::TableListBox tapTable; //Declaración de una instancia del objeto tapTable de la clase de JUCE TableListBox 

    juce::TextButton addTapButton { "+" }; //Declaración de una instancia del objeto addTapbUTTON de la clase de JUCE TextButton

    MultiTapDelay& delay; //Referencia del delay

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TableEditor)
};