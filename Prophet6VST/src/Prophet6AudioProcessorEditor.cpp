#include "Prophet6AudioProcessorEditor.h"

Prophet6AudioProcessorEditor::Prophet6AudioProcessorEditor(Prophet6Processor& p)
    : AudioProcessorEditor(&p), processor(p),
      keyboardComponent(processor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 600);

    addAndMakeVisible(oscMixSlider);
    addAndMakeVisible(osc1FreqSlider);
    addAndMakeVisible(osc2FreqSlider);
    addAndMakeVisible(filterCutoffSlider);
    addAndMakeVisible(filterResonanceSlider);
    addAndMakeVisible(envAttackSlider);
    addAndMakeVisible(envDecaySlider);
    addAndMakeVisible(envSustainSlider);
    addAndMakeVisible(envReleaseSlider);
    addAndMakeVisible(lfoRateSlider);
    addAndMakeVisible(lfoAmountSlider);

    addAndMakeVisible(osc1WaveformCombo);
    addAndMakeVisible(osc2WaveformCombo);
    addAndMakeVisible(lfoWaveformCombo);

    addAndMakeVisible(arpOnButton);
    addAndMakeVisible(arpModeCombo);
    addAndMakeVisible(arpRateSlider);

    addAndMakeVisible(effectsOnButton);
    addAndMakeVisible(distortionAmountSlider);
    addAndMakeVisible(delayTimeSlider);
    addAndMakeVisible(delayFeedbackSlider);
    addAndMakeVisible(reverbSizeSlider);

    addAndMakeVisible(keyboardComponent);

    createSlider(oscMixSlider, "oscMix");
    createSlider(osc1FreqSlider, "osc1Freq");
    createSlider(osc2FreqSlider, "osc2Freq");
    createSlider(filterCutoffSlider, "filterCutoff");
    createSlider(filterResonanceSlider, "filterResonance");
    createSlider(envAttackSlider, "envAttack");
    createSlider(envDecaySlider, "envDecay");
    createSlider(envSustainSlider, "envSustain");
    createSlider(envReleaseSlider, "envRelease");
    createSlider(lfoRateSlider, "lfoRate");
    createSlider(lfoAmountSlider, "lfoAmount");

    createComboBox(osc1WaveformCombo, "osc1Waveform");
    createComboBox(osc2WaveformCombo, "osc2Waveform");
    createComboBox(lfoWaveformCombo, "lfoWaveform");

    createToggleButton(arpOnButton, "arpOn");
    createComboBox(arpModeCombo, "arpMode");
    createSlider(arpRateSlider, "arpRate");

    createToggleButton(effectsOnButton, "effectsOn");
    createSlider(distortionAmountSlider, "distortionAmount");
    createSlider(delayTimeSlider, "delayTime");
    createSlider(delayFeedbackSlider, "delayFeedback");
    createSlider(reverbSizeSlider, "reverbSize");
}

Prophet6AudioProcessorEditor::~Prophet6AudioProcessorEditor()
{
}

void Prophet6AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void Prophet6AudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    auto topArea = area.removeFromTop(area.getHeight() * 0.8f);
    auto bottomArea = area;

    auto oscArea = topArea.removeFromLeft(topArea.getWidth() * 0.4f);
    auto filterEnvArea = topArea.removeFromLeft(topArea.getWidth() * 0.5f);
    auto lfoEffectsArea = topArea;

    oscMixSlider.setBounds(oscArea.removeFromTop(50));
    osc1FreqSlider.setBounds(oscArea.removeFromTop(50));
    osc2FreqSlider.setBounds(oscArea.removeFromTop(50));
    osc1WaveformCombo.setBounds(oscArea.removeFromTop(30));
    osc2WaveformCombo.setBounds(oscArea.removeFromTop(30));

    filterCutoffSlider.setBounds(filterEnvArea.removeFromTop(50));
    filterResonanceSlider.setBounds(filterEnvArea.removeFromTop(50));
    envAttackSlider.setBounds(filterEnvArea.removeFromTop(50));
    envDecaySlider.setBounds(filterEnvArea.removeFromTop(50));
    envSustainSlider.setBounds(filterEnvArea.removeFromTop(50));
    envReleaseSlider.setBounds(filterEnvArea.removeFromTop(50));

    lfoRateSlider.setBounds(lfoEffectsArea.removeFromTop(50));
    lfoAmountSlider.setBounds(lfoEffectsArea.removeFromTop(50));
    lfoWaveformCombo.setBounds(lfoEffectsArea.removeFromTop(30));

    arpOnButton.setBounds(lfoEffectsArea.removeFromTop(30));
    arpModeCombo.setBounds(lfoEffectsArea.removeFromTop(30));
    arpRateSlider.setBounds(lfoEffectsArea.removeFromTop(50));

    effectsOnButton.setBounds(lfoEffectsArea.removeFromTop(30));
    distortionAmountSlider.setBounds(lfoEffectsArea.removeFromTop(50));
    delayTimeSlider.setBounds(lfoEffectsArea.removeFromTop(50));
    delayFeedbackSlider.setBounds(lfoEffectsArea.removeFromTop(50));
    reverbSizeSlider.setBounds(lfoEffectsArea.removeFromTop(50));

    keyboardComponent.setBounds(bottomArea);
}

void Prophet6AudioProcessorEditor::createSlider(juce::Slider& slider, const juce::String& parameterID)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    slider.setPopupDisplayEnabled(true, false, this);
    slider.setTextValueSuffix(processor.getParameters()[parameterID]->getLabel());

    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getAPVTS(), parameterID, slider));
}

void Prophet6AudioProcessorEditor::createComboBox(juce::ComboBox& comboBox, const juce::String& parameterID)
{
    comboBox.addItemList(processor.getParameters()[parameterID]->getAllValueStrings(), 1);

    comboBoxAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processor.getAPVTS(), parameterID, comboBox));
}

void Prophet6AudioProcessorEditor::createToggleButton(juce::ToggleButton& button, const juce::String& parameterID)
{
    button.setButtonText(processor.getParameters()[parameterID]->getName(100));

    buttonAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processor.getAPVTS(), parameterID, button));
}