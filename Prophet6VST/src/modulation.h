#pragma once

#include <JuceHeader.h>
#include <vector>
#include <functional>

class LFO
{
public:
    LFO();
    ~LFO();

    void init(double sampleRate);
    float process();

    enum class Waveform
    {
        Sine,
        Triangle,
        Saw,
        Square,
        Random
    };

    void setWaveform(Waveform waveform);
    void setRate(float rate);
    void setDepth(float depth);

private:
    double sampleRate;
    double phase;
    Waveform currentWaveform;
    float rate;
    float depth;

    float generateWaveform();
};

class ModulationMatrix
{
public:
    ModulationMatrix();
    ~ModulationMatrix();

    void init();
    void process();

    enum class Source
    {
        LFO1,
        LFO2,
        ModWheel,
        Aftertouch,
        Envelope1,
        Envelope2
    };

    enum class Destination
    {
        Osc1Pitch,
        Osc2Pitch,
        FilterCutoff,
        Amplitude,
        PulseWidth
    };

    void addModulation(Source source, Destination destination, float amount);
    void clearModulations();

private:
    struct ModulationRoute
    {
        Source source;
        Destination destination;
        float amount;
    };

    std::vector<ModulationRoute> routes;
    std::vector<float> sourceValues;
    std::vector<float> destinationValues;

    void updateSourceValues();
    void applyModulations();
};