#include "effects.h"
#include <cmath>

Distortion::Distortion() : amount(0.5f) {}

void Distortion::process(float& sample)
{
    sample = std::tanh(sample * (1.0f + 15.0f * amount));
}

void Distortion::setAmount(float newAmount)
{
    amount = juce::jlimit(0.0f, 1.0f, newAmount);
}

Delay::Delay() : writePosition(0), time(0.5f), feedback(0.5f), mix(0.5f), sampleRate(44100.0) {}

void Delay::init(double sampleRate)
{
    this->sampleRate = sampleRate;
    buffer.setSize(2, static_cast<int>(sampleRate * 2.0)); // 2 seconds maximum delay
    buffer.clear();
}

void Delay::process(float& left, float& right)
{
    const int bufferLength = buffer.getNumSamples();
    const int delayLength = static_cast<int>(time * sampleRate);
    const int readPosition = (writePosition - delayLength + bufferLength) % bufferLength;

    float delayLeft = buffer.getSample(0, readPosition);
    float delayRight = buffer.getSample(1, readPosition);

    buffer.setSample(0, writePosition, left + delayLeft * feedback);
    buffer.setSample(1, writePosition, right + delayRight * feedback);

    writePosition = (writePosition + 1) % bufferLength;

    left = left * (1.0f - mix) + delayLeft * mix;
    right = right * (1.0f - mix) + delayRight * mix;
}

void Delay::setTime(float newTime)
{
    time = juce::jlimit(0.0f, 2.0f, newTime);
}

void Delay::setFeedback(float newFeedback)
{
    feedback = juce::jlimit(0.0f, 0.95f, newFeedback);
}

void Delay::setMix(float newMix)
{
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}

Reverb::Reverb()
{
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.33f;
    params.dryLevel = 0.4f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
}

void Reverb::init(double sampleRate)
{
    reverb.setSampleRate(sampleRate);
}

void Reverb::process(float& left, float& right)
{
    float* channels[] = { &left, &right };
    reverb.processStereo(channels[0], channels[1], 1);
}

void Reverb::setRoomSize(float size)
{
    params.roomSize = juce::jlimit(0.0f, 1.0f, size);
    reverb.setParameters(params);
}

void Reverb::setDamping(float damping)
{
    params.damping = juce::jlimit(0.0f, 1.0f, damping);
    reverb.setParameters(params);
}

void Reverb::setMix(float mix)
{
    params.wetLevel = juce::jlimit(0.0f, 1.0f, mix);
    params.dryLevel = 1.0f - mix;
    reverb.setParameters(params);
}

Effects::Effects() {}

void Effects::init(double sampleRate)
{
    delay.init(sampleRate);
    reverb.init(sampleRate);
}

void Effects::process(float& left, float& right)
{
    distortion.process(left);
    distortion.process(right);
    delay.process(left, right);
    reverb.process(left, right);
}