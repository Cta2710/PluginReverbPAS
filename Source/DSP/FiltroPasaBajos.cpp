#include "FiltroPasaBajos.h"

FiltroPasaBajos::FiltroPasaBajos(){}

void FiltroPasaBajos::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    updateCoefficient();
}

void FiltroPasaBajos::reset()
{
    z1 = 0.0f;
}

void FiltroPasaBajos::setCutFreq(float freq)
{
    cutFreq = freq;
    updateCoefficient();
}

void FiltroPasaBajos::updateCoefficient()
{
    constexpr float pi = 3.14159265359f;
    float dt = 1.0f / currentSampleRate;
    float RC = 1.0f / (2.0f * pi * cutFreq);

    alpha = dt / (RC + dt);
}

float FiltroPasaBajos::processSample(float x)
{
    z1 += alpha * (x - z1);
    return z1;
}