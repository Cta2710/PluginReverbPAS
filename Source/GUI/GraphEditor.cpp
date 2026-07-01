#pragma once
#include "GraphEditor.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
GraphEditor::GraphEditor(MultiTapDelay& d,  //Tiene una referencia del MultiTap, que se la pasa PluginEditor al cual se la pasa PluginProcessor
                        Parameter parameter) //en el mismo constructor defino el parámetro que el gráfio va a representar/controlar
                                            //Con esto puedo reutilizar la clase mediante composición, una sola implementación de la clase con varias instancias especializadas
    : delay(d), 
     parameter(parameter) //Lista de inicialización
{}

float GraphEditor::getValueForTap(int tap) const //Recibe el índice del tap y devuelve un número
{
    switch (parameter)
    {
        case Parameter::Delay:
            return delay.getTap(tap).delayMs; //Devuelve el miembro retardo        

        case Parameter::Gain:
            return delay.getTap(tap).gain; //Devuelve el miembro ganancia

        case Parameter::CutOffFreq:
            return delay.getTap(tap).cutOffFreq; //Devuelve el miembro frecuencia de corte
    }

    return 0.0f; //Sino, devuelve cero
}

void GraphEditor::setValueForTap(int tap, float value) //
{
    switch (parameter)
    {
        case Parameter::Delay:
            delay.getTap(tap).delayMs = static_cast<int>(value); //modifico desde la referencia
            break;

        case Parameter::Gain:
            delay.getTap(tap).gain = value;
            break;

         case Parameter::CutOffFreq:
            delay.getTap(tap).cutOffFreq = value;
            delay.getTap(tap).lowpass.setCutFreq(value);
            break;
    }

    repaint();
}

void GraphEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);

    switch (parameter)
    {
        case Parameter::Delay:
           g.drawText("Delay Time (ms: max 5000)",
           getLocalBounds().removeFromTop(20),
           juce::Justification::centred);            
           break;

        case Parameter::Gain:
           g.drawText("Gain (max 1)",
           getLocalBounds().removeFromTop(20),
           juce::Justification::centred);            
           break;

         case Parameter::CutOffFreq:
           g.drawText("Cut-off Frequency (LINEAR SCALE)",
           getLocalBounds().removeFromTop(20),
           juce::Justification::centred);            
           break;
    }


    for (int i = 0; i < delay.getNumTaps(); ++i)
    {
        auto p = getPointPosition(i);  

        g.fillEllipse(p.x - 5.0f,  //dibuja los círculos
                      p.y - 5.0f,
                      10.0f,
                      10.0f);

        if (i > 0)
        {
            auto prev = getPointPosition(i - 1);

            g.drawLine(prev.x, //dibuja las líneas
                       prev.y,
                       p.x,
                       p.y,
                       2.0f);
        }

        repaint();
    }
}

juce::Point<float> GraphEditor::getPointPosition(int tap) const 
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);

    // Evitar división por cero si solo hay 1 tap
    float spacing = 0.0f;
    if (delay.getNumTaps() > 1)
        spacing = area.getWidth() / static_cast<float>(delay.getNumTaps() - 1);

    float x = area.getX() + static_cast<float>(tap) * spacing;
    float value = getValueForTap(tap);
    float y = valueToY(value); 

    return { x, y };
}

float GraphEditor::valueToY(float value) const //
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);

    float maxValue = 1.0f;
    switch (parameter) //Define valor máximo para cada parámetro
    {
        case Parameter::Delay:
            maxValue = 5000.0f;
            break;

        case Parameter::Gain:
            maxValue = 1.0f;
            break;

        case Parameter::CutOffFreq:
            maxValue = 20000.0f;
            break;
    }

    return juce::jmap(value,
                      0.0f,
                      maxValue,
                      area.getBottom(),
                      area.getY());
}

float GraphEditor::yToValue(float y) const
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);
    
    float maxValue;
    switch (parameter) //Define valor máximo para cada parámetro
    {
        case Parameter::Delay:
            maxValue = 5000.0f;
            break;

        case Parameter::Gain:
            maxValue = 1.0f;
            break;

        case Parameter::CutOffFreq:
            maxValue = 20000.0f;
            break;
    }

    return juce::jlimit(
        0.0f,
        maxValue,
        juce::jmap(y,
                   area.getBottom(),
                   area.getY(),
                   0.0f,
                   maxValue));
}

void GraphEditor::mouseDown(const juce::MouseEvent& event)
{
    selectedPoint = -1; // Reseteamos por las dudas

    // Buscamos si el click fue cerca de algún punto
    for (int i = 0; i < delay.getNumTaps(); ++i)
    {
        auto p = getPointPosition(i);
        if (event.position.getDistanceFrom(p) < 12.0f) // 12px es un buen margen de error para el click
        {
            selectedPoint = i;
            break;
        }
    }
}

void GraphEditor::mouseDrag(const juce::MouseEvent& event)
{
    // Si agarramos un punto en mouseDown, le actualizamos el valor al arrastrar
    if (selectedPoint >= 0)
    {
        float newValue = yToValue(event.position.y);
        setValueForTap(selectedPoint, newValue);
    }
    if (onDataChanged)
    onDataChanged();
}

void GraphEditor::mouseUp(const juce::MouseEvent& event)
{
    selectedPoint = -1; // Soltamos el punto
}

void GraphEditor::resized() {}