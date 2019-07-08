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
                                                              std::make_unique<AudioParameterFloat>(wavetable_ParamIDs[kWtbParam_Position], "Wavetable Position", NormalisableRange<float> (0.0f, 0.99999f), 0.0f),
                                                              std::make_unique<AudioParameterBool>(wavetable_ParamIDs[kWtbParam_Interpolate], "Interpolate", false),
                                                              std::make_unique<AudioParameterInt>(wavetable_ParamIDs[kWtbParam_Semitones], "Wavetable Semitones", -36, 36, 0),
                                                              std::make_unique<AudioParameterFloat>(wavetable_ParamIDs[kWtbParam_Cents], "Wavetable Cents", NormalisableRange<float> (-1.0f, 1.0f), 0.0f),
                                                              std::make_unique<AudioParameterFloat>(wavetable_ParamIDs[kWtbParam_Volume], "Wavetable Volume", NormalisableRange<float> (0.0f, 1.0f), 1.0f)));
    
    // amp envelope
    layout.add(std::make_unique<AudioProcessorParameterGroup>("AmpEnvelopeGroupID", "Amp Envelope", "/",
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Attack],
                                                                                                    "Attack",
                                                                                                    NormalisableRange<float> (0.1f,
                                                                                                                              20.0f,
                                                                                                                              [](float start, float end, float value) { return convertToRangeWithAnchor(start, end, value, 0.5, 1.0);},
                                                                                                                              [](float start, float end, float value) { return convertFromRangeWithAnchor(start, end, value, 0.5, 1.0);}),
                                                                                                    0.1f),
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Decay], "Decay", NormalisableRange<float> (0.1f,
                                                                                                                                                                           20.0f,
                                                                                                                                                                           [](float start, float end, float value) { return convertToRangeWithAnchor(start, end, value, 0.5, 1.0);},
                                                                                                                                                                           [](float start, float end, float value) { return convertFromRangeWithAnchor(start, end, value, 0.5, 1.0);}),
                                                                                                    0.1f),
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Sustain], "Sustain", NormalisableRange<float> (0.0f, 1.0f), 0.7f),
                                                              std::make_unique<AudioParameterFloat>(envelope_ParamIDs[kEnvParam_Release], "Release", NormalisableRange<float> (0.1f,
                                                                                                                                                                               20.0f,
                                                                                                                                                                               [](float start, float end, float value) { return convertToRangeWithAnchor(start, end, value, 0.5, 1.0);},
                                                                                                                                                                               [](float start, float end, float value) { return convertFromRangeWithAnchor(start, end, value, 0.5, 1.0);}),
                                                                                                    0.1f)));
    
    // amplifier
    layout.add(std::make_unique<AudioProcessorParameterGroup>("AmplifierID", "Amp", "/",
                                                              std::make_unique<AudioParameterFloat>(amplifier_ParamIDs[kAmpParam_Gain], "Gain", NormalisableRange<float> (0.0f, 1.0f), 1.0f),
                                                              std::make_unique<AudioParameterFloat>(amplifier_ParamIDs[kAmpParam_Pan], "Pan", NormalisableRange<float> (-1.0f, 1.0f), 0.0f)));
                                                              
    // lfo 1
    layout.add(std::make_unique<AudioProcessorParameterGroup>("LfoGroupID", "LFO", "/",
                                                              std::make_unique<AudioParameterInt>(lfo_ParamIDs[kLfoParam_WaveType], "LFO1 Wavetype", 1, 5, 1),
                                                              std::make_unique<AudioParameterInt>(lfo_ParamIDs[kLfoParam_RunState], "LFO1 Run Type", 1, 5, 1),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_PulseWidth], "LFO1 Pulse Width", NormalisableRange<float> (1.0f, 99.0f), 50.0f),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_PhaseOffset], "LFO1 Phase Offset", NormalisableRange<float> (0.0f, 1.0f), 0.0f),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_Amount], "LFO1 Amount", NormalisableRange<float> (0.0f, 1.0f), 1.0f),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_PolarityOffset], "LFO1 Polarity Offset", NormalisableRange<float> (-1.0f, 1.0f), 0.0f),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_Rate], "LFO1 Rate", NormalisableRange<float> (1.0f, 20.0f), 1.0f),
                                                              std::make_unique<AudioParameterBool>(lfo_ParamIDs[kLfoParam_Sync], "LFO1 Sync", false),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_FadeInTime], "LFO1 Fade In Time", NormalisableRange<float> (0.0f, 1.0f), 0.0f),
                                                              std::make_unique<AudioParameterFloat>(lfo_ParamIDs[kLfoParam_Delay], "LFO1 Delay", NormalisableRange<float> (0.0f, 1.0f), 0.0f)));

    // output gain
    layout.add(std::make_unique<AudioProcessorParameterGroup>("OutputGroupID", "Output Gain", "/",
                                                              std::make_unique<AudioParameterFloat>(outputGain_ParameterID, "Output Gain", NormalisableRange<float> (0.0f, 1.0f), Decibels::decibelsToGain(0.0f))));
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
    
    // lfo
    state.addParameterListener(lfo_ParamIDs[kLfoParam_WaveType], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_RunState], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_PulseWidth], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_PhaseOffset], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_Amount], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_PolarityOffset], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_Rate], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_Sync], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_FadeInTime], this);
    state.addParameterListener(lfo_ParamIDs[kLfoParam_Delay], this);

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

    // LFO
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_WaveType], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_RunState], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_PulseWidth], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_PhaseOffset], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_Amount], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_PolarityOffset], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_Rate], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_Sync], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_FadeInTime], this);
    state.removeParameterListener(lfo_ParamIDs[kLfoParam_Delay], this);

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

