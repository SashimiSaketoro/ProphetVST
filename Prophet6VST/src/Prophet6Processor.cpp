#include "Prophet6Processor.h"

Prophet6Processor::Prophet6Processor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    initializeParameters();
}

Prophet6Processor::~Prophet6Processor()
{
}

const juce::String Prophet6Processor::getName() const
{
    return JucePlugin_Name;
}

bool Prophet6Processor::acceptsMidi() const
{
    return true;
}

bool Prophet6Processor::producesMidi() const
{
    return false;
}

bool Prophet6Processor::isMidiEffect() const
{
    return false;
}

double Prophet6Processor::getTailLengthSeconds() const
{
    return 0.0;
}

int Prophet6Processor::getNumPrograms()
{
    return 1;
}

int Prophet6Processor::getCurrentProgram()
{
    return 0;
}

void Prophet6Processor::setCurrentProgram(int index)
{
}

const juce::String Prophet6Processor::getProgramName(int index)
{
    return {};
}

void Prophet6Processor::changeProgramName(int index, const juce::String& newName)
{
}

void Prophet6Processor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    osc1.init(sampleRate);
    osc2.init(sampleRate);
    subOsc.init(sampleRate);
    filter.init(sampleRate);
    amp.init(sampleRate);
    modMatrix.init();
    arp.init(sampleRate);
    seq.init(sampleRate);
    effects.init(sampleRate);
}

void Prophet6Processor::releaseResources()
{
}

void Prophet6Processor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float oscMix = apvts.getRawParameterValue("oscMix")->load();
    float osc1Freq = apvts.getRawParameterValue("osc1Freq")->load();
    float osc2Freq = apvts.getRawParameterValue("osc2Freq")->load();
    int osc1Waveform = static_cast<int>(apvts.getRawParameterValue("osc1Waveform")->load());
    int osc2Waveform = static_cast<int>(apvts.getRawParameterValue("osc2Waveform")->load());
    float filterCutoff = apvts.getRawParameterValue("filterCutoff")->load();
    float filterResonance = apvts.getRawParameterValue("filterResonance")->load();
    float envAttack = apvts.getRawParameterValue("envAttack")->load();
    float envDecay = apvts.getRawParameterValue("envDecay")->load();
    float envSustain = apvts.getRawParameterValue("envSustain")->load();
    float envRelease = apvts.getRawParameterValue("envRelease")->load();
    float lfoRate = apvts.getRawParameterValue("lfoRate")->load();
    float lfoAmount = apvts.getRawParameterValue("lfoAmount")->load();
    int lfoWaveform = static_cast<int>(apvts.getRawParameterValue("lfoWaveform")->load());
    bool arpOn = apvts.getRawParameterValue("arpOn")->load() > 0.5f;
    int arpMode = static_cast<int>(apvts.getRawParameterValue("arpMode")->load());
    float arpRate = apvts.getRawParameterValue("arpRate")->load();
    bool effectsOn = apvts.getRawParameterValue("effectsOn")->load() > 0.5f;
    float distortionAmount = apvts.getRawParameterValue("distortionAmount")->load();
    float delayTime = apvts.getRawParameterValue("delayTime")->load();
    float delayFeedback = apvts.getRawParameterValue("delayFeedback")->load();
    float reverbSize = apvts.getRawParameterValue("reverbSize")->load();

    osc1.setFrequency(osc1Freq);
    osc1.setWaveform(static_cast<Oscillator::Waveform>(osc1Waveform));
    osc2.setFrequency(osc2Freq);
    osc2.setWaveform(static_cast<Oscillator::Waveform>(osc2Waveform));
    filter.setCutoff(filterCutoff);
    filter.setResonance(filterResonance);
    amp.setAttack(envAttack);
    amp.setDecay(envDecay);
    amp.setSustain(envSustain);
    amp.setRelease(envRelease);
    modMatrix.setLFORate(lfoRate);
    modMatrix.setLFOAmount(lfoAmount);
    modMatrix.setLFOWaveform(static_cast<LFO::Waveform>(lfoWaveform));
    arp.setOn(arpOn);
    arp.setMode(static_cast<Arpeggiator::Mode>(arpMode));
    arp.setRate(arpRate);
    effects.setOn(effectsOn);
    effects.distortion.setAmount(distortionAmount);
    effects.delay.setTime(delayTime);
    effects.delay.setFeedback(delayFeedback);
    effects.reverb.setSize(reverbSize);

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float left = 0.0f, right = 0.0f;

        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (voices[v].active)
            {
                float oscOutput = oscMix * osc1.process(voices[v].frequency) +
                                  (1.0f - oscMix) * osc2.process(voices[v].frequency);
                
                float filterOutput = filter.process(oscOutput);
                float ampOutput = amp.process(filterOutput);

                left += ampOutput;
                right += ampOutput;
            }
        }

        modMatrix.process();

        if (arpOn)
        {
            arp.process(left, right);
        }

        if (effectsOn)
        {
            effects.process(left, right);
        }

        buffer.setSample(0, sample, left);
        buffer.setSample(1, sample, right);
    }
}

bool Prophet6Processor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Prophet6Processor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

void Prophet6Processor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Prophet6Processor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void Prophet6Processor::processMidiEvents(const juce::MidiBuffer& midiMessages)
{
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        if (message.isNoteOn())
        {
            noteOn(message.getNoteNumber(), message.getFloatVelocity());
        }
        else if (message.isNoteOff())
        {
            noteOff(message.getNoteNumber());
        }
        else if (message.isController())
        {
            // Handle MIDI CC messages
        }
        else if (message.isMidiMachineControlMessage())
        {
            handleMidi2Event(message);
        }
    }
}

void Prophet6Processor::handleMidi2Event(const juce::MidiMessage& message)
{
    // Handle MIDI 2.0 specific messages
    if (message.isMidiMachineControlMessage())
    {
        switch (message.getMidiMachineControlCommand())
        {
            case juce::MidiMessage::mmc_play:
                // Start playback
                break;
            case juce::MidiMessage::mmc_stop:
                // Stop playback
                break;
            case juce::MidiMessage::mmc_pause:
                // Pause playback
                break;
            // Add more cases for other MIDI 2.0 commands as needed
        }
    }
    else if (message.isNoteOn())
    {
        // Handle MIDI 2.0 note on with higher resolution
        int noteNumber = message.getNoteNumber();
        float velocity = message.getFloatVelocity();
        noteOn(noteNumber, velocity);
    }
    else if (message.isNoteOff())
    {
        // Handle MIDI 2.0 note off with higher resolution
        int noteNumber = message.getNoteNumber();
        noteOff(noteNumber);
    }
    else if (message.isController())
    {
        // Handle MIDI 2.0 controllers with higher resolution
        int controllerNumber = message.getControllerNumber();
        float value = message.getControllerValue() / 127.0f;
        // Update the appropriate parameter based on the controller number
        // For example:
        // if (controllerNumber == 1) // Modulation wheel
        //     modMatrix.setModulationAmount(value);
    }
}

int Prophet6Processor::findFreeVoice()
{
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (!voices[v].active)
        {
            return v;
        }
    }
    return -1; // No free voices
}

void Prophet6Processor::noteOn(int midiNoteNumber, float velocity)
{
    int voice = findFreeVoice();
    if (voice != -1)
    {
        voices[voice].active = true;
        voices[voice].noteNumber = midiNoteNumber;
        voices[voice].frequency = 440.0f * std::pow(2.0f, (midiNoteNumber - 69) / 12.0f);
        // Initialize other voice parameters
    }
}

void Prophet6Processor::noteOff(int midiNoteNumber)
{for (int v = 0; v < MAX_VOICES; v++)
{
    if (voices[v].active && voices[v].noteNumber == midiNoteNumber)
    {
        voices[v].active = false;
        // Handle voice release
    }
}
}

void Prophet6Processor::initializeParameters()
{
    for (auto* param : getParameters())
    {
        parameters[param->getParameterID()] = param;
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout Prophet6Processor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("oscMix", "Osc Mix", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Freq", "Osc 1 Frequency", -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc2Freq", "Osc 2 Frequency", -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc1Waveform", "Osc 1 Waveform", juce::StringArray("Saw", "Square", "Triangle"), 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc2Waveform", "Osc 2 Waveform", juce::StringArray("Saw", "Square", "Triangle"), 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterCutoff", "Filter Cutoff", 20.0f, 20000.0f, 1000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterResonance", "Filter Resonance", 0.0f, 1.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("envAttack", "Envelope Attack", 0.001f, 5.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("envDecay", "Envelope Decay", 0.001f, 5.0f, 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("envSustain", "Envelope Sustain", 0.0f, 1.0f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("envRelease", "Envelope Release", 0.001f, 5.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("lfoRate", "LFO Rate", 0.1f, 20.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("lfoAmount", "LFO Amount", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("lfoWaveform", "LFO Waveform", juce::StringArray("Sine", "Triangle", "Saw", "Square", "Random"), 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("arpOn", "Arpeggiator On", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("arpMode", "Arp Mode", juce::StringArray("Up", "Down", "Up/Down", "Random"), 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("arpRate", "Arp Rate", 0.1f, 20.0f, 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("effectsOn", "Effects On", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distortionAmount", "Distortion Amount", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time", 0.0f, 2.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delayFeedback", "Delay Feedback", 0.0f, 0.95f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverbSize", "Reverb Size", 0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Prophet6Processor();
}