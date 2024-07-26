#pragma once

#include <JuceHeader.h>

class Filter
{
public:
    Filter();
    ~Filter();

    void init(double sampleRate);
    float process(float input);

    void setCutoff(float frequency);
    void setResonance(float resonance);
    void setEnvelopeAmount(float amount);
    void setKeyboardTracking(float tracking);

    enum class Type
    {
        LowPass,
        HighPass,
        BandPass
    };

    void setType(Type type);

private:
    double sampleRate;
    float cutoff;
    float resonance;
    float envelopeAmount;
    float keyboardTracking;
    Type currentType;

    // State variables for the filter
    float z1, z2;

    // Coefficients
    float a0, a1, a2, b1, b2;

    void calculateCoefficients();
};