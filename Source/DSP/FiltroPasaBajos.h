#pragma once

// Esta clase construye un filtro pasa-bajos de primer orden
// Usa la fórmula: y[n]=y[n−1]+α(x[n]−y[n−1])
class FiltroPasaBajos 
{
public:

    FiltroPasaBajos();
    
    void prepare(double sampleRate);
    void setCutFreq(float freq);
    void reset();

    float processSample(float x);

private:
    double currentSampleRate = 44100.0;
    float cutFreq = 2000.0f;

    float alpha = 0.0f;
    float z1 = 0.0f; // y[n-1]

    void updateCoefficient();
};
