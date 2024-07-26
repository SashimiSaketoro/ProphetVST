#include "amplifier.h"
#include <cmath>

Amplifier::Amplifier()
    : sampleRate(44100.0), attack(0.01f), decay(0.1f), sustain(0.7f), release(0.2f),
      envelopeLevel(0.0f), isActive(false), currentStage(EnvelopeStage::Idle)
{
}

Amplifier::~Amplifier()
{
}

void Amplifier::init(double sampleRate)
{
    this->sampleRate = sampleRate;
}

float Amplifier::process(float input)
{
    float envelopeValue = calculateEnvelope();
    return input * envelopeValue;
}

void Amplifier::setAttack(float attackTime)
{
    attack = juce::jlimit(0.001f, 5.0f, attackTime);
}

void Amplifier::setDecay(float decayTime)
{
    decay = juce::jlimit(0.001f, 5.0f, decayTime);
}

void Amplifier::setSustain(float sustainLevel)
{
    sustain = juce::jlimit(0.0f, 1.0f, sustainLevel);
}

void Amplifier::setRelease(float releaseTime)
{
    release = juce::jlimit(0.001f, 5.0f, releaseTime);
}

void Amplifier::noteOn()
{
    isActive = true;
    currentStage = EnvelopeStage::Attack;
    envelopeLevel = 0.0f;
}

void Amplifier::noteOff()
{
    isActive = false;
    currentStage = EnvelopeStage::Release;
}

float Amplifier::calculateEnvelope()
{
    float envelopeIncrement;

    switch (currentStage)
    {
        case EnvelopeStage::Idle:
            return 0.0f;

        case EnvelopeStage::Attack:
            envelopeIncrement = 1.0f / (attack * sampleRate);
            envelopeLevel += envelopeIncrement;
            if (envelopeLevel >= 1.0f)
            {
                envelopeLevel = 1.0f;
                currentStage = EnvelopeStage::Decay;
            }
            break;

        case EnvelopeStage::Decay:
            envelopeIncrement = (sustain - 1.0f) / (decay * sampleRate);
            envelopeLevel += envelopeIncrement;
            if (envelopeLevel <= sustain)
            {
                envelopeLevel = sustain;
                currentStage = EnvelopeStage::Sustain;
            }
            break;

        case EnvelopeStage::Sustain:
            envelopeLevel = sustain;
            break;

        case EnvelopeStage::Release:
            envelopeIncrement = -sustain / (release * sampleRate);
            envelopeLevel += envelopeIncrement;
            if (envelopeLevel <= 0.0f)
            {
                envelopeLevel = 0.0f;
                currentStage = EnvelopeStage::Idle;
            }
            break;
    }

    return envelopeLevel;
}