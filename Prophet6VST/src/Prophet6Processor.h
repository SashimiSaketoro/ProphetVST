#pragma once

#include <JuceHeader.h>
#include "oscillator.h"
#include "filter.h"
#include "amplifier.h"
#include "modulation.h"
#include "arpeggiator.h"
#include "sequencer.h"
#include "effects.h"

class Prophet6Processor : public juce::AudioProcessor
{
public:
    Prophet6Processor();
    ~Prophet6Processor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::MidiKeyboardState keyboardState;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiMessageCollector& midiCollector);

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    const std::map<juce::String, juce::AudioProcessorParameter*>& getParameters() const { return parameters; }

private:
    // Prophet 6 components
    Oscillator osc1;
    Oscillator osc2;
    SubOscillator subOsc;
    Filter filter;
    Amplifier amp;
    ModulationMatrix modMatrix;
    Arpeggiator arp;
    Sequencer seq;
    Effects effects;

    // MIDI handling
    void processMidiEvents(const juce::MidiBuffer& midiMessages);
    void handleMidi2Event(const juce::MidiMessage& message);

    // Voice management
    static const int MAX_VOICES = 6;
    struct Voice {
        bool active;
        int noteNumber;
        float frequency;
        // Add other voice-specific data
    };
    Voice voices[MAX_VOICES];
    int findFreeVoice();
    void noteOn(int midiNoteNumber, float velocity);
    void noteOff(int midiNoteNumber);

    juce::AudioProcessorValueTreeState apvts;
    std::map<juce::String, juce::AudioProcessorParameter*> parameters;

    void initializeParameters();
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Prophet6Processor)
};