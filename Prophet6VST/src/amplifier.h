#pragma once

#include <JuceHeader.h>

class Amplifier
{
public:
    Amplifier();
    ~Amplifier();

    void init(double sampleRate);
    float process(float input);

    void setAttack(float attackTime);
    void setDecay(float decayTime);
    void setSustain(float sustainLevel);
    void setRelease(float releaseTime);

    void noteOn();
    void noteOff();

private:
    double sampleRate;
    float attack, decay, sustain, release;
    float envelopeLevel;
    bool isActive;

    enum class EnvelopeStage
    {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };

    EnvelopeStage currentStage;

    float calculateEnvelope();
};