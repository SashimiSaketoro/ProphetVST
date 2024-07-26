#include "Prophet6Processor.h"
#include "Prophet6AudioProcessorEditor.h"

class Prophet6Plugin : public juce::AudioProcessor
{
public:
    Prophet6Plugin() : processor() {}

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        processor.prepareToPlay(sampleRate, samplesPerBlock);
    }

    void releaseResources() override
    {
        processor.releaseResources();
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        processor.processBlock(buffer, midiMessages);
    }

    bool hasEditor() const override { return true; }

    juce::AudioProcessorEditor* createEditor() override
    {
        return new Prophet6AudioProcessorEditor(processor);
    }

    void getStateInformation(juce::MemoryBlock& destData) override
    {
        auto state = processor.getAPVTS().copyState();
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        copyXmlToBinary(*xml, destData);
    }

    void setStateInformation(const void* data, int sizeInBytes) override
    {
        std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
        if (xmlState.get() != nullptr)
        {
            if (xmlState->hasTagName(processor.getAPVTS().state.getType()))
            {
                processor.getAPVTS().replaceState(juce::ValueTree::fromXml(*xmlState));
            }
        }
    }

private:
    Prophet6Processor processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Prophet6Plugin)
};

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Prophet6Plugin();
}