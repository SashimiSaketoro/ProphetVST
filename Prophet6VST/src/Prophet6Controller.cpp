#include "Prophet6Controller.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ustring.h"

using namespace Steinberg;

Prophet6Controller::Prophet6Controller()
{
}

tresult PLUGIN_API Prophet6Controller::initialize(FUnknown* context)
{
    tresult result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
    {
        return result;
    }

    // Create parameters
    createOscillatorParameters();
    createFilterParameters();
    createAmplifierParameters();
    createModulationParameters();
    createArpeggiatorParameters();
    createSequencerParameters();
    createEffectsParameters();

    return kResultOk;
}

tresult PLUGIN_API Prophet6Controller::setComponentState(IBStream* state)
{
    // Sync controller with processor state
    return kResultOk;
}

tresult PLUGIN_API Prophet6Controller::setParamNormalized(Vst::ParamID tag, Vst::ParamValue value)
{
    tresult result = EditControllerEx1::setParamNormalized(tag, value);
    return result;
}

void Prophet6Controller::createOscillatorParameters()
{
    parameters.addParameter(STR16("Osc1 Waveform"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kOsc1Waveform);
    parameters.addParameter(STR16("Osc1 Frequency"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kOsc1Frequency);
    parameters.addParameter(STR16("Osc1 Fine Tune"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kOsc1FineTune);
    parameters.addParameter(STR16("Osc2 Waveform"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kOsc2Waveform);
    parameters.addParameter(STR16("Osc2 Frequency"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kOsc2Frequency);
    parameters.addParameter(STR16("Osc2 Fine Tune"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kOsc2FineTune);
    parameters.addParameter(STR16("Osc Sync"), nullptr, 1, 0, Vst::ParameterInfo::kCanAutomate, kOscSync);
    parameters.addParameter(STR16("Sub Osc Level"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kSubOscLevel);
}

void Prophet6Controller::createFilterParameters()
{
    parameters.addParameter(STR16("Filter Cutoff"), nullptr, 0, 1, Vst::ParameterInfo::kCanAutomate, kFilterCutoff);
    parameters.addParameter(STR16("Filter Resonance"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kFilterResonance);
    parameters.addParameter(STR16("Filter Env Amount"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kFilterEnvAmount);
    parameters.addParameter(STR16("Filter Keyboard Tracking"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kFilterKeyboardTracking);
}

void Prophet6Controller::createAmplifierParameters()
{
    parameters.addParameter(STR16("Amp Attack"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kAmpAttack);
    parameters.addParameter(STR16("Amp Decay"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kAmpDecay);
    parameters.addParameter(STR16("Amp Sustain"), nullptr, 0, 1, Vst::ParameterInfo::kCanAutomate, kAmpSustain);
    parameters.addParameter(STR16("Amp Release"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kAmpRelease);
}

void Prophet6Controller::createModulationParameters()
{
    parameters.addParameter(STR16("LFO1 Rate"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kLFO1Rate);
    parameters.addParameter(STR16("LFO1 Shape"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kLFO1Shape);
    parameters.addParameter(STR16("LFO2 Rate"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kLFO2Rate);
    parameters.addParameter(STR16("LFO2 Shape"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kLFO2Shape);
}

void Prophet6Controller::createArpeggiatorParameters()
{
    parameters.addParameter(STR16("Arp On/Off"), nullptr, 1, 0, Vst::ParameterInfo::kCanAutomate, kArpOn);
    parameters.addParameter(STR16("Arp Mode"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kArpMode);
    parameters.addParameter(STR16("Arp Octave"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kArpOctave);
}

void Prophet6Controller::createSequencerParameters()
{
    parameters.addParameter(STR16("Seq On/Off"), nullptr, 1, 0, Vst::ParameterInfo::kCanAutomate, kSeqOn);
    parameters.addParameter(STR16("Seq Steps"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kSeqSteps);
}

void Prophet6Controller::createEffectsParameters()
{
    parameters.addParameter(STR16("Distortion Amount"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kDistortionAmount);
    parameters.addParameter(STR16("Delay Time"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kDelayTime);
    parameters.addParameter(STR16("Delay Feedback"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kDelayFeedback);
    parameters.addParameter(STR16("Reverb Size"), nullptr, 0, 0.5, Vst::ParameterInfo::kCanAutomate, kReverbSize);
    parameters.addParameter(STR16("Reverb Mix"), nullptr, 0, 0.3, Vst::ParameterInfo::kCanAutomate, kReverbMix);
}