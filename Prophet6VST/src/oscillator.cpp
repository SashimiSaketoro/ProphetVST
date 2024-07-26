#include "oscillator.h"
#include <cmath>

Oscillator::Oscillator()
    : sampleRate(44100.0), phase(0.0), currentWaveform(Waveform::Saw), pulseWidth(0.5f), detune(0.0f)
{
}

Oscillator::~Oscillator()
{
}

void Oscillator::init(double sampleRate)
{
    this->sampleRate = sampleRate;
    phase = 0.0;
}

float Oscillator::process(float frequency)
{
    double increment = frequency / sampleRate;
    phase += increment;
    if (phase >= 1.0)
        phase -= 1.0;

    float output = 0.0f;
    switch (currentWaveform)
    {
        case Waveform::Saw:
            output = generateSaw(phase);
            break;
        case Waveform::Square:
            output = generateSquare(phase);
            break;
        case Waveform::Triangle:
            output = generateTriangle(phase);
            break;
    }

    // Apply detune
    if (detune != 0.0f)
    {
        double detunePhase = phase + detune;
        if (detunePhase >= 1.0)
            detunePhase -= 1.0;
        output = (output + generateSaw(detunePhase)) * 0.5f;
    }

    return output;
}

void Oscillator::setWaveform(Waveform waveform)
{
    currentWaveform = waveform;
}

void Oscillator::setPulseWidth(float pw)
{
    pulseWidth = juce::jlimit(0.1f, 0.9f, pw);
}

void Oscillator::setDetune(float det)
{
    detune = det;
}

float Oscillator::generateSaw(float t)
{
    return 2.0f * t - 1.0f;
}

float Oscillator::generateSquare(float t)
{
    return (t < pulseWidth) ? 1.0f : -1.0f;
}

float Oscillator::generateTriangle(float t)
{
    return (t < 0.5f) ? (4.0f * t - 1.0f) : (3.0f - 4.0f * t);
}

SubOscillator::SubOscillator()
    : sampleRate(44100.0), phase(0.0), level(1.0f)
{
}

SubOscillator::~SubOscillator()
{
}

void SubOscillator::init(double sampleRate)
{
    this->sampleRate = sampleRate;
    phase = 0.0;
}

float SubOscillator::process(float frequency)
{
    double increment = (frequency * 0.5) / sampleRate;  // One octave lower
    phase += increment;
    if (phase >= 1.0)
        phase -= 1.0;

    return level * (phase < 0.5 ? 1.0f : -1.0f);  // Square wave
}

void SubOscillator::setLevel(float l)
{
    level = juce::jlimit(0.0f, 1.0f, l);
}