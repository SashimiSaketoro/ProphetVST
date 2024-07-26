#pragma once

#include <JuceHeader.h>

class Distortion
{
public:
    Distortion();
    void process(float& sample);
    void setAmount(float amount);

private:
    float amount;
};

class Delay
{
public:
    Delay();
    void init(double sampleRate);
    void process(float& left, float& right);
    void setTime(float time);
    void setFeedback(float feedback);
    void setMix(float mix);

private:
    juce::AudioSampleBuffer buffer;
    int writePosition;
    float time;
    float feedback;
    float mix;
    double sampleRate;
};

class Reverb
{
public:
    Reverb();
    void init(double sampleRate);
    void process(float& left, float& right);
    void setRoomSize(float size);
    void setDamping(float damping);
    void setMix(float mix);

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
};

class Effects
{
public:
    Effects();
    void init(double sampleRate);
    void process(float& left, float& right);

    Distortion distortion;
    Delay delay;
    Reverb reverb;
};