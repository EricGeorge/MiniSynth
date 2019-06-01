/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthVoice.h"
#include "OscillatorParameters.h"

SynthVoice::SynthVoice()
:   level(0.0f),
    osc1(getSampleRate()),
    osc2(getSampleRate()),
    lfo1(getSampleRate())
{
}

SynthVoice::~SynthVoice()
{
}

void SynthVoice::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID.contains(oscillator1ParamIDPrefix))
    {
        osc1ParameterChanged(parameterID, newValue);
    }
    else if (parameterID.contains(oscillator2ParamIDPrefix))
    {
        osc2ParameterChanged(parameterID, newValue);
    }
    else if (parameterID.contains(lfo1ParamIDPrefix))
    {
        lfo1ParameterChanged(parameterID, newValue);
    }
}

void SynthVoice::osc1ParameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == oscillator1_ParamIDs[kOscParam_WaveType])
    {
        osc1.setWaveType(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_Octave])
    {
        osc1.setOctaves(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_Semitone])
    {
        osc1.setSemitones(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_Cents])
    {
        osc1.setCents(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_PulseWidth])
    {
        osc1.setPulseWidth(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_PolyBLEPMix])
    {
        osc1.setPolyBLEPMix(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_WaveShapeSaturation])
    {
        osc1.setWaveShapeSaturation(newValue);
    }
    else if (parameterID == oscillator1_ParamIDs[kOscParam_Volume])
    {
        osc1.setVolume(newValue);
    }
}

void SynthVoice::osc2ParameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == oscillator2_ParamIDs[kOscParam_WaveType])
    {
        osc2.setWaveType(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_Octave])
    {
        osc2.setOctaves(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_Semitone])
    {
        osc2.setSemitones(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_Cents])
    {
        osc2.setCents(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_PulseWidth])
    {
        osc2.setPulseWidth(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_PolyBLEPMix])
    {
        osc2.setPolyBLEPMix(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_WaveShapeSaturation])
    {
        osc2.setWaveShapeSaturation(newValue);
    }
    else if (parameterID == oscillator2_ParamIDs[kOscParam_Volume])
    {
        osc2.setVolume(newValue);
    }
}

void SynthVoice::lfo1ParameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == lfo1_ParamIDs[kLfoParam_WaveType])
    {
        lfo1.setWaveType(newValue);
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_RunState])
    {
        lfo1.setRunState(newValue);     // TODO
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_PulseWidth])
    {
        lfo1.setPulseWidth(newValue);
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_PhaseOffset])
    {
        lfo1.setPhaseOffset(newValue);  // TODO
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_Amount])
    {
        lfo1.setAmount(newValue);
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_PolarityOffset])
    {
        lfo1.setPolarityOffset(newValue);   // TODO
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_Rate])
    {
        lfo1.setRate(newValue);     // TODO
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_Sync])
    {
        lfo1.setSync(newValue);     //TODO
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_FadeInTime])
    {
        lfo1.setFadeInTime(newValue);   // TODO
    }
    else if (parameterID == lfo1_ParamIDs[kLfoParam_Delay])
    {
        lfo1.setDelay(newValue);    // TODO
    }
}


bool SynthVoice::canPlaySound(SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity,
                           SynthesiserSound* sound, int currentPitchWheelPosition)
{
    level = velocity * 0.25;

    osc1.startNote(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    osc2.startNote(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    osc1.stopNote();
    osc2.stopNote();
    clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    for (int index = 0; index < numSamples; ++index)
    {
        double sample1 = osc1.getNextSample() * level;
        double sample2 = osc2.getNextSample() * level;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample, sample1 * osc1.getVolume() + sample2 * osc2.getVolume());
        }
        ++startSample;
    }
}
