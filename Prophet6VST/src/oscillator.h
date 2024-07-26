#pragma once

#include <JuceHeader.h>

class Oscillator
{
public:
    Oscillator();
    ~Oscillator();

    void init(double sampleRate);
    float process(float frequency);

    enum class Waveform
    {
        Saw,
        Square,
        Triangle
    };

    void setWaveform(Waveform waveform);
    void setPulseWidth(float pulseWidth);
    void setDetune(float detune);

private:
    double sampleRate;
    double phase;
    Waveform currentWaveform;
    float pulseWidth;
    float detune;

    float generateSaw(float t);
    float generateSquare(float t);
    float generateTriangle(float t);
};

class SubOscillator
{
public:
    SubOscillator();
    ~SubOscillator();

    void init(double sampleRate);
    float process(float frequency);

    void setLevel(float level);

private:
    double sampleRate;
    double phase;
    float level;
};