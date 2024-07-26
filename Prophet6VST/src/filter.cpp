#include "filter.h"
#include <cmath>

Filter::Filter()
    : sampleRate(44100.0), cutoff(1000.0f), resonance(0.5f), envelopeAmount(0.0f), keyboardTracking(0.0f),
      currentType(Type::LowPass), z1(0.0f), z2(0.0f)
{
    calculateCoefficients();
}

Filter::~Filter()
{
}

void Filter::init(double sampleRate)
{
    this->sampleRate = sampleRate;
    calculateCoefficients();
}

float Filter::process(float input)
{
    float output = a0 * input + z1;
    z1 = a1 * input - b1 * output + z2;
    z2 = a2 * input - b2 * output;
    return output;
}

void Filter::setCutoff(float frequency)
{
    cutoff = juce::jlimit(20.0f, static_cast<float>(sampleRate / 2.0f), frequency);
    calculateCoefficients();
}

void Filter::setResonance(float res)
{
    resonance = juce::jlimit(0.0f, 0.99f, res);
    calculateCoefficients();
}

void Filter::setEnvelopeAmount(float amount)
{
    envelopeAmount = amount;
}

void Filter::setKeyboardTracking(float tracking)
{
    keyboardTracking = juce::jlimit(0.0f, 1.0f, tracking);
}

void Filter::setType(Type type)
{
    currentType = type;
    calculateCoefficients();
}

void Filter::calculateCoefficients()
{
    float omega = 2.0f * M_PI * cutoff / static_cast<float>(sampleRate);
    float cosOmega = std::cos(omega);
    float alpha = std::sin(omega) / (2.0f * resonance);

    switch (currentType)
    {
        case Type::LowPass:
            b1 = -2.0f * cosOmega;
            b2 = 1.0f - alpha;
            a0 = (1.0f - cosOmega) / 2.0f;
            a1 = 1.0f - cosOmega;
            a2 = a0;
            break;

        case Type::HighPass:
            b1 = -2.0f * cosOmega;
            b2 = 1.0f - alpha;
            a0 = (1.0f + cosOmega) / 2.0f;
            a1 = -(1.0f + cosOmega);
            a2 = a0;
            break;

        case Type::BandPass:
            b1 = -2.0f * cosOmega;
            b2 = 1.0f - alpha;
            a0 = alpha;
            a1 = 0.0f;
            a2 = -a0;
            break;
    }

    // Normalize coefficients
    float a0inv = 1.0f / (1.0f + alpha);
    a0 *= a0inv;
    a1 *= a0inv;
    a2 *= a0inv;
    b1 *= a0inv;
    b2 *= a0inv;
}