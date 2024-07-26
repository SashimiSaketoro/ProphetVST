#pragma once

#include <JuceHeader.h>
#include "Prophet6Processor.h"

class MainComponent  : public juce::AudioAppComponent
{
public:
    MainComponent()
    {
        addAndMakeVisible(keyboardComponent);
        setSize (600, 400);
        setAudioChannels (0, 2);
    }

    ~MainComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        prophet6.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        prophet6.processBlock(bufferToFill.buffer, midiCollector);
    }

    void releaseResources() override
    {
        prophet6.releaseResources();
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto area = getLocalBounds();
        keyboardComponent.setBounds (area.removeFromBottom (80));
    }

private:
    Prophet6Processor prophet6;
    juce::MidiKeyboardComponent keyboardComponent {prophet6.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard};
    juce::MidiMessageCollector midiCollector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};