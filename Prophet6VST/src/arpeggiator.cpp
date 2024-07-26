#include "arpeggiator.h"
#include <algorithm>
#include <random>

Arpeggiator::Arpeggiator()
    : sampleRate(44100.0), currentMode(Mode::Up), rate(4.0f), octaveRange(1), gateTime(0.5f),
      isRunning(false), currentStep(0), stepCounter(0.0)
{
}

Arpeggiator::~Arpeggiator()
{
}

void Arpeggiator::init(double sampleRate)
{
    this->sampleRate = sampleRate;
}

void Arpeggiator::process(float& left, float& right)
{
    if (!isRunning || heldNotes.empty())
        return;

    stepCounter += rate / sampleRate;
    if (stepCounter >= 1.0)
    {
        stepCounter -= 1.0;
        advanceStep();
    }

    // Here you would typically trigger the note on/off events
    // and apply the gate time. For simplicity, we'll just modify
    // the output amplitude based on the gate time.
    float amplitude = (stepCounter < gateTime) ? 1.0f : 0.0f;
    left *= amplitude;
    right *= amplitude;
}

void Arpeggiator::setMode(Mode mode)
{
    currentMode = mode;
    generateSequence();
}

void Arpeggiator::setRate(float newRate)
{
    rate = juce::jlimit(0.1f, 20.0f, newRate);
}

void Arpeggiator::setOctaveRange(int range)
{
    octaveRange = juce::jlimit(1, 4, range);
    generateSequence();
}

void Arpeggiator::setGateTime(float newGateTime)
{
    gateTime = juce::jlimit(0.1f, 0.9f, newGateTime);
}

void Arpeggiator::noteOn(int midiNote, float velocity)
{
    heldNotes.push_back(midiNote);
    std::sort(heldNotes.begin(), heldNotes.end());
    generateSequence();
}

void Arpeggiator::noteOff(int midiNote)
{
    heldNotes.erase(std::remove(heldNotes.begin(), heldNotes.end(), midiNote), heldNotes.end());
    if (heldNotes.empty())
        stop();
    else
        generateSequence();
}

void Arpeggiator::start()
{
    isRunning = true;
    currentStep = 0;
    stepCounter = 0.0;
}

void Arpeggiator::stop()
{
    isRunning = false;
}

void Arpeggiator::generateSequence()
{
    sequence.clear();
    if (heldNotes.empty())
        return;

    switch (currentMode)
    {
        case Mode::Up:
            for (int octave = 0; octave < octaveRange; ++octave)
                for (int note : heldNotes)
                    sequence.push_back(note + octave * 12);
            break;

        case Mode::Down:
            for (int octave = octaveRange - 1; octave >= 0; --octave)
                for (auto it = heldNotes.rbegin(); it != heldNotes.rend(); ++it)
                    sequence.push_back(*it + octave * 12);
            break;

        case Mode::UpDown:
            for (int octave = 0; octave < octaveRange; ++octave)
                for (int note : heldNotes)
                    sequence.push_back(note + octave * 12);
            if (octaveRange > 1 || heldNotes.size() > 1)
                for (int octave = octaveRange - 1; octave >= 0; --octave)
                    for (auto it = std::next(heldNotes.rbegin()); it != heldNotes.rend(); ++it)
                        sequence.push_back(*it + octave * 12);
            break;

        case Mode::Random:
            {
                std::vector<int> allNotes;
                for (int octave = 0; octave < octaveRange; ++octave)
                    for (int note : heldNotes)
                        allNotes.push_back(note + octave * 12);
                
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(allNotes.begin(), allNotes.end(), g);
                sequence = allNotes;
            }
            break;

        case Mode::AsPlayed:
            sequence = heldNotes;
            break;
    }
}

void Arpeggiator::advanceStep()
{
    if (sequence.empty())
        return;

    currentStep = (currentStep + 1) % sequence.size();
}

int Arpeggiator::getCurrentNote()
{
    if (sequence.empty())
        return -1;

    return sequence[currentStep];
}