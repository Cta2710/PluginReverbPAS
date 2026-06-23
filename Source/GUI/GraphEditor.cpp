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
    }

    return 0.0f; //Sino, devuelve cero
}

void GraphEditor::setValueForTap(int tap, float value) //
{
    switch (parameter)
    {
        case Parameter::Delay:
            delay.getTap(tap).delayMs = value; //modifico desde la referencia
            break;

        case Parameter::Gain:
            delay.getTap(tap).gain = value;
            break;
    }

    repaint();
}

void GraphEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);

    for (int i = 0; i < delay.getNumTaps(); ++i)
    {
        auto p = getPointPosition(i);  

        g.fillEllipse(p.x - 4.0f,  //dibuja los círculos
                      p.y - 4.0f,
                      8.0f,
                      8.0f);

        if (i > 0)
        {
            auto prev = getPointPosition(i - 1);

            g.drawLine(prev.x, //dibuja las líneas
                       prev.y,
                       p.x,
                       p.y,
                       2.0f);
        }
    }
}

juce::Point<float> GraphEditor::getPointPosition(int tap) const  //Calcula posición en la pantalla de cada tap
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);

    float spacing = area.getWidth() / (delay.getNumTaps() - 1);

    float x = area.getX() + tap * spacing;

    float value = getValueForTap(tap);

    float y = valueToY(value); 

    return { x, y };
}

float GraphEditor::valueToY(float value) const //
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);

    return juce::jmap(value,
                      0.0f,
                      1.0f,
                      area.getBottom(),
                      area.getY());
}

float GraphEditor::yToValue(float y) const
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);

    return juce::jlimit(
        0.0f,
        1.0f,
        juce::jmap(y,
                   area.getBottom(),
                   area.getY(),
                   0.0f,
                   1.0f));
}

void GraphEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (selectedPoint < 0)
        return;

    float value = yToValue(event.position.y);

    setValueForTap(selectedPoint, value);

    int selectedPoint = -1;

    for (int i = 0; i < delay.getNumTaps(); ++i)
    {
        auto p = getPointPosition(i);

        if (event.position.getDistanceFrom(p) < 8.0f)
        {
            selectedPoint = i;
            break;
        }
    }
}