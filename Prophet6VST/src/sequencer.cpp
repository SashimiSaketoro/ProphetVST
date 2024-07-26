#include "sequencer.h"

Sequencer::Sequencer()
    : sampleRate(44100.0), rate(4.0f), gateTime(0.5f), running(false),
      currentStep(0), stepCounter(0.0), gateCounter(0.0)
{
}

Sequencer::~Sequencer()
{
}

void Sequencer::init(double sampleRate)
{
    this->sampleRate = sampleRate;
}

void Sequencer::process(float& left, float& right)
{
    if (!running || sequence.empty())
        return;

    stepCounter += rate / sampleRate;
    if (stepCounter >= 1.0)
    {
        stepCounter -= 1.0;
        advanceStep();
    }

    gateCounter += 1.0 / sampleRate;
    if (gateCounter >= gateTime)
    {
        releaseNote();
    }

    // Here you would typically modify the output based on the current step
    // For simplicity, we'll just modify the amplitude
    float amplitude = (gateCounter < gateTime) ? 1.0f : 0.0f;
    left *= amplitude;
    right *= amplitude;
}

void Sequencer::setSteps(const std::vector<Step>& steps)
{
    sequence = steps;
    reset();
}

void Sequencer::setRate(float newRate)
{
    rate = juce::jlimit(0.1f, 20.0f, newRate);
}

void Sequencer::setGateTime(float newGateTime)
{
    gateTime = juce::jlimit(0.1f, 0.9f, newGateTime);
}

void Sequencer::start()
{
    running = true;
    reset();
}

void Sequencer::stop()
{
    running = false;
    releaseNote();
}

void Sequencer::reset()
{
    currentStep = 0;
    stepCounter = 0.0;
    gateCounter = 0.0;
}

void Sequencer::advanceStep()
{
    if (sequence.empty())
        return;

    currentStep = (currentStep + 1) % sequence.size();
    gateCounter = 0.0;

    const Step& step = sequence[currentStep];
    if (!step.rest)
    {
        triggerNote(step);
    }
    else
    {
        releaseNote();
    }
}

void Sequencer::triggerNote(const Step& step)
{
    // Here you would typically trigger the note in your synthesizer
    // For this example, we'll just print the note information
    juce::Logger::writeToLog("Triggered note: " + juce::String(step.note) + 
                             " with velocity: " + juce::String(step.velocity));
}

void Sequencer::releaseNote()
{
    // Here you would typically release the note in your synthesizer
    juce::Logger::writeToLog("Released note");
}