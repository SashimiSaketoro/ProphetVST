#include "modulation.h"
#include <cmath>
#include <random>

LFO::LFO()
    : sampleRate(44100.0), phase(0.0), currentWaveform(Waveform::Sine), rate(1.0f), depth(1.0f)
{
}

LFO::~LFO()
{
}

void LFO::init(double sampleRate)
{
    this->sampleRate = sampleRate;
    phase = 0.0;
}

float LFO::process()
{
    double increment = rate / sampleRate;
    phase += increment;
    if (phase >= 1.0)
        phase -= 1.0;

    return depth * generateWaveform();
}

void LFO::setWaveform(Waveform waveform)
{
    currentWaveform = waveform;
}

void LFO::setRate(float newRate)
{
    rate = juce::jlimit(0.1f, 20.0f, newRate);
}

void LFO::setDepth(float newDepth)
{
    depth = juce::jlimit(0.0f, 1.0f, newDepth);
}

float LFO::generateWaveform()
{
    switch (currentWaveform)
    {
        case Waveform::Sine:
            return std::sin(2.0 * M_PI * phase);
        case Waveform::Triangle:
            return 2.0f * std::fabs(2.0f * phase - 1.0f) - 1.0f;
        case Waveform::Saw:
            return 2.0f * phase - 1.0f;
        case Waveform::Square:
            return (phase < 0.5f) ? 1.0f : -1.0f;
        case Waveform::Random:
            {
                static std::default_random_engine generator;
                static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
                return distribution(generator);
            }
        default:
            return 0.0f;
    }
}

ModulationMatrix::ModulationMatrix()
{
}

ModulationMatrix::~ModulationMatrix()
{
}

void ModulationMatrix::init()
{
    routes.clear();
    sourceValues.resize(static_cast<size_t>(Source::Envelope2) + 1, 0.0f);
    destinationValues.resize(static_cast<size_t>(Destination::PulseWidth) + 1, 0.0f);
}

void ModulationMatrix::process()
{
    updateSourceValues();
    applyModulations();
}

void ModulationMatrix::addModulation(Source source, Destination destination, float amount)
{
    routes.push_back({source, destination, amount});
}

void ModulationMatrix::clearModulations()
{
    routes.clear();
}

void ModulationMatrix::updateSourceValues()
{
    // In a real implementation, you would update these values from your LFOs, envelopes, etc.
    // For now, we'll just use dummy values
    sourceValues[static_cast<size_t>(Source::LFO1)] = std::sin(2.0 * M_PI * 0.1);
    sourceValues[static_cast<size_t>(Source::LFO2)] = std::sin(2.0 * M_PI * 0.2);
    sourceValues[static_cast<size_t>(Source::ModWheel)] = 0.5f;
    sourceValues[static_cast<size_t>(Source::Aftertouch)] = 0.0f;
    sourceValues[static_cast<size_t>(Source::Envelope1)] = 0.7f;
    sourceValues[static_cast<size_t>(Source::Envelope2)] = 0.3f;
}

void ModulationMatrix::applyModulations()
{
    // Reset destination values
    std::fill(destinationValues.begin(), destinationValues.end(), 0.0f);

    // Apply all modulation routes
    for (const auto& route : routes)
    {
        size_t sourceIndex = static_cast<size_t>(route.source);
        size_t destIndex = static_cast<size_t>(route.destination);
        destinationValues[destIndex] += sourceValues[sourceIndex] * route.amount;
    }

    // In a real implementation, you would now use these destination values to modulate your synth parameters
}