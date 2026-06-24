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
            delay.getTap(tap).delayMs = static_cast<int>(value); //modifico desde la referencia
            break;

        case Parameter::Gain:
            delay.getTap(tap).gain = value;
            break;
    }

    repaint();
}

void GraphEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);

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

    float maxValue = (parameter == Parameter::Delay) ? 5000.0f : 1.0f;

    return juce::jmap(value,
                      0.0f,
                      maxValue,
                      area.getBottom(),
                      area.getY());
}

float GraphEditor::yToValue(float y) const
{
    auto area = getLocalBounds().toFloat().reduced(graphMargin);
    float maxValue = (parameter == Parameter::Delay) ? 5000.0f : 1.0f; // <-- Rango dinámico

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
}

void GraphEditor::mouseUp(const juce::MouseEvent& event)
{
    selectedPoint = -1; // Soltamos el punto
}

void GraphEditor::resized()
{
}