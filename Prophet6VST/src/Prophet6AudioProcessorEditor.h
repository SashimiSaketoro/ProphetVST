#pragma once

#include <JuceHeader.h>
#include "Prophet6Processor.h"

class Prophet6AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    Prophet6AudioProcessorEditor(Prophet6Processor&);
    ~Prophet6AudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    Prophet6Processor& processor;

    juce::Slider oscMixSlider;
    juce::Slider osc1FreqSlider;
    juce::Slider osc2FreqSlider;
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    juce::Slider envAttackSlider;
    juce::Slider envDecaySlider;
    juce::Slider envSustainSlider;
    juce::Slider envReleaseSlider;
    juce::Slider lfoRateSlider;
    juce::Slider lfoAmountSlider;

    juce::ComboBox osc1WaveformCombo;
    juce::ComboBox osc2WaveformCombo;
    juce::ComboBox lfoWaveformCombo;

    juce::ToggleButton arpOnButton;
    juce::ComboBox arpModeCombo;
    juce::Slider arpRateSlider;

    juce::ToggleButton effectsOnButton;
    juce::Slider distortionAmountSlider;
    juce::Slider delayTimeSlider;
    juce::Slider delayFeedbackSlider;
    juce::Slider reverbSizeSlider;

    juce::MidiKeyboardComponent keyboardComponent;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>> comboBoxAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;

    void createSlider(juce::Slider& slider, const juce::String& parameterID);
    void createComboBox(juce::ComboBox& comboBox, const juce::String& parameterID);
    void createToggleButton(juce::ToggleButton& button, const juce::String& parameterID);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Prophet6AudioProcessorEditor)
};