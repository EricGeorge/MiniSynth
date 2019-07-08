/*
  ==============================================================================

    Synth.cpp
    Created: 20 May 2019 10:21:21am
    Author:  Eric

  ==============================================================================
*/

#include "Synth.h"

#include "AmplifierParameters.h"
#include "EnvelopeParameters.h"
#include "OutputParameters.h"
#include "OscillatorParameters.h"
#include "WavetableParameters.h"

#include "PluginHelpers.h"
#include "SynthVoice.h"

Synth::Synth()
{
    addSound(new SynthSound());

    for (int i = 0; i < kNumVoices; ++i)
    {
        addVoice(new SynthVoice(*this));
    }
}

Synth::~Synth()
{
    
}

SynthSound& Synth::getSynthSound() const
{
    return *dynamic_cast<SynthSound*>(getSound(0).get());
}

void Synth::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    // wavetable
    layout.add(std::make_unique<AudioProcessorParameterGroup>("WavetableOscGroupID", "Wavetable Osc", "/",
                                                              std::make_unique<AudioParameterFloat>(wavetable_ParamIDs[kWtbParam_Position],
                                                                                                    "Wavetable Position",
                                                                                                    NormalisableRange<float> (wtbPositionMinValue,
                                                                                                                              wtbPositionMaxValue),
                                                                                                    wtbPositionInitialValue),
                                                              std::make_unique<AudioParameterBool>(wavetable_ParamIDs[kWtbParam_Interpolate],
                                                                                                   "Interpolate",
                                                                                                   wtbInterpolateInitialValue),
                                                              std::make_unique<AudioParameterInt>(wavetable_ParamIDs[kWtbParam_Semitones],
                                                                                                  "Wavetable Semitones",
                                                                                                  wtbSemitonesMinValue,
                                                                                                  wtbSemitonesMaxValue,
                                                                                                  wtbSemitonesInitialValue),
                                                              std::make_unique<AudioParameterFloat>(wavetable_ParamIDs[kWtbParam_Cents],
                                                                                                    "Wavetable Cents",
                                                                                                    NormalisableRange<float> (wtbCentsMinValue,
                                                                                                                              wtbCentsMaxValue),
                                                                                                    wtbCentsInitialValue),
                                                              std::make_unique<AudioParameterFloat>(wavetable_ParamIDs[kWtbParam_Volume],
                                                                                                    "Wavetable Volume",
                                                                                                    NormalisableRange<float> (wtbVolumeMinValue,
                                                                                                                              wtbVolumeMaxValue),
                                                                                                    wtbVolumeInitialValue)));
    
    // amp envelope
    layout.add(std::make_unique<AudioProcessorParameterGroup>("AmpEnvelopeGroupID", "Amp Envelope", "/",
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Attack],
                                                                                                    "Attack",
                                                                                                    NormalisableRange<float> (envAttackMinValue,
                                                                                                                              envAttackMaxValue,
                                                                                                                              [](float start, float end, float value) { return convertToRangeWithAnchor(start, end, value, 0.5, 1.0);},
                                                                                                                              [](float start, float end, float value) { return convertFromRangeWithAnchor(start, end, value, 0.5, 1.0);}),
                                                                                                    envAttackInitialValue),
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Decay],
                                                                                                    "Decay",
                                                                                                    NormalisableRange<float> (envDecayMinValue,
                                                                                                                              envDecayMaxValue,
                                                                                                                              [](float start, float end, float value) { return convertToRangeWithAnchor(start, end, value, 0.5, 1.0);},
                                                                                                                              [](float start, float end, float value) { return convertFromRangeWithAnchor(start, end, value, 0.5, 1.0);}),
                                                                                                    envDecayInitialValue),
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Sustain],
                                                                                                    "Sustain",
                                                                                                    NormalisableRange<float> (envSustainMinValue,
                                                                                                                              envSustainMaxValue),
                                                                                                    envSustainInitialValue),
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Release],
                                                                                                    "Release",
                                                                                                    NormalisableRange<float> (envReleaseMinValue,
                                                                                                                              envReleaseMaxValue,
                                                                                                                              [](float start, float end, float value) { return convertToRangeWithAnchor(start, end, value, 0.5, 1.0);},
                                                                                                                              [](float start, float end, float value) { return convertFromRangeWithAnchor(start, end, value, 0.5, 1.0);}),
                                                                                                    envReleaseInitialValue)));
    
    // amplifier
    layout.add(std::make_unique<AudioProcessorParameterGroup>("AmplifierID", "Amp", "/",
                                                              std::make_unique<AudioParameterFloat>(amplifier_ParamIDs[kAmpParam_Gain],
                                                                                                    "Gain",
                                                                                                    NormalisableRange<float> (ampGainMinValue,
                                                                                                                              ampGainMaxValue),
                                                                                                    ampGainInitialValue),
                                                              std::make_unique<AudioParameterFloat>(amplifier_ParamIDs[kAmpParam_Pan],
                                                                                                    "Pan",
                                                                                                    NormalisableRange<float> (ampPanMinValue,
                                                                                                                              ampPanMaxValue),
                                                                                                    ampPanInitialValue)));
    
    // output gain
    layout.add(std::make_unique<AudioProcessorParameterGroup>("OutputGroupID", "Output Gain", "/",
                                                              std::make_unique<AudioParameterFloat>(outputGain_ParameterID,
                                                                                                    "Output Gain",
                                                                                                    NormalisableRange<float> (outputGainMinValue,
                                                                                                                              outputGainMaxValue),
                                                                                                    Decibels::decibelsToGain(outputGainInitialValue))));
}

void Synth::addParameterListeners(AudioProcessorValueTreeState& state)
{
    // wavetable
    state.addParameterListener(wavetable_ParamIDs[kWtbParam_Position], this);
    state.addParameterListener(wavetable_ParamIDs[kWtbParam_Interpolate], this);
    state.addParameterListener(wavetable_ParamIDs[kWtbParam_Semitones], this);
    state.addParameterListener(wavetable_ParamIDs[kWtbParam_Cents], this);
    state.addParameterListener(wavetable_ParamIDs[kWtbParam_Volume], this);

    // amp envelope
    state.addParameterListener(envelope_ParamIDs[kEnvParam_Attack], this);
    state.addParameterListener(envelope_ParamIDs[kEnvParam_Decay], this);
    state.addParameterListener(envelope_ParamIDs[kEnvParam_Sustain], this);
    state.addParameterListener(envelope_ParamIDs[kEnvParam_Release], this);

    // amp
    state.addParameterListener(amplifier_ParamIDs[kAmpParam_Gain], this);
    state.addParameterListener(amplifier_ParamIDs[kAmpParam_Pan], this);
    
    // output gain
    state.addParameterListener(outputGain_ParameterID, this);
}
    
void Synth::removeParameterListeners(AudioProcessorValueTreeState& state)
{
   // oscillator 2
    state.removeParameterListener(wavetable_ParamIDs[kWtbParam_Position], this);
    state.removeParameterListener(wavetable_ParamIDs[kWtbParam_Interpolate], this);
    state.removeParameterListener(wavetable_ParamIDs[kWtbParam_Semitones], this);
    state.removeParameterListener(wavetable_ParamIDs[kWtbParam_Cents], this);
    state.removeParameterListener(wavetable_ParamIDs[kWtbParam_Volume], this);
    
    // amp envelope
    state.removeParameterListener(envelope_ParamIDs[kEnvParam_Attack], this);
    state.removeParameterListener(envelope_ParamIDs[kEnvParam_Decay], this);
    state.removeParameterListener(envelope_ParamIDs[kEnvParam_Sustain], this);
    state.removeParameterListener(envelope_ParamIDs[kEnvParam_Release], this);
    
    // amp
    state.removeParameterListener(amplifier_ParamIDs[kAmpParam_Gain], this);
    state.removeParameterListener(amplifier_ParamIDs[kAmpParam_Pan], this);

    //output gain
    state.removeParameterListener(outputGain_ParameterID, this);
}

void Synth::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == outputGain_ParameterID)
    {
        outputGain.setGain(newValue);
    }
    else
    {
        for (int i = 0; i < getNumVoices(); ++i)
        {
            dynamic_cast<SynthVoice*>(getVoice(i))->parameterChanged(parameterID, newValue);
        }
    }
}

void Synth::renderNextBlock (AudioBuffer<float>& outputAudio,
                      const MidiBuffer& inputMidi,
                      int startSample,
                      int numSamples)
{
    // render the voices first
    Synthesiser::renderNextBlock(outputAudio, inputMidi, startSample, numSamples);
    
    // and then all of the external processors
    outputGain.process(outputAudio, outputAudio.getNumSamples());
}

