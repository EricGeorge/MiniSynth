/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthVoice.h"

#include "OscillatorParameters.h"
#include "SynthSound.h"
#include "WavetableParameters.h"

SynthVoice::SynthVoice(Synth& synth)
:   synth(synth),
    level(0.0f),
    osc(getSampleRate()),
    wtb(getSampleRate(), synth.getSynthSound()),
    lfo(getSampleRate())
{
}

SynthVoice::~SynthVoice()
{
}

void SynthVoice::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID.contains(oscillatorParamIDPrefix))
    {
        oscParameterChanged(parameterID, newValue);
    }
    else if (parameterID.contains(wavetableParamIDPrefix))
    {
        wtbParameterChanged(parameterID, newValue);
    }
    else if (parameterID.contains(lfoParamIDPrefix))
    {
        lfoParameterChanged(parameterID, newValue);
    }
}

void SynthVoice::oscParameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == oscillator_ParamIDs[kOscParam_WaveType])
    {
        osc.setWaveType(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_Octave])
    {
        osc.setOctaves(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_Semitone])
    {
        osc.setSemitones(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_Cents])
    {
        osc.setCents(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_PulseWidth])
    {
        osc.setPulseWidth(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_PolyBLEPMix])
    {
        osc.setPolyBLEPMix(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_WaveShapeSaturation])
    {
        osc.setWaveShapeSaturation(newValue);
    }
    else if (parameterID == oscillator_ParamIDs[kOscParam_Volume])
    {
        osc.setVolume(newValue);
    }
}

void SynthVoice::wtbParameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == wavetable_ParamIDs[kWtbParam_Position])
    {
        wtb.setPosition(newValue);
    }
    else if (parameterID == wavetable_ParamIDs[kWtbParam_Interpolate])
    {
        wtb.setInterpolate(newValue);
    }
    else if (parameterID == wavetable_ParamIDs[kWtbParam_Semitones])
    {
        wtb.setSemitones(newValue);
    }
    else if (parameterID == wavetable_ParamIDs[kWtbParam_Cents])
    {
        wtb.setCents(newValue);
    }
    else if (parameterID == wavetable_ParamIDs[kWtbParam_Volume])
    {
        wtb.setVolume(newValue);
    }
}

void SynthVoice::lfoParameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == lfo_ParamIDs[kLfoParam_WaveType])
    {
        lfo.setWaveType(newValue);
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_RunState])
    {
        lfo.setRunState(newValue);     // TODO
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_PulseWidth])
    {
        lfo.setPulseWidth(newValue);
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_PhaseOffset])
    {
        lfo.setPhaseOffset(newValue);  // TODO
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_Amount])
    {
        lfo.setAmount(newValue);
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_PolarityOffset])
    {
        lfo.setPolarityOffset(newValue);   // TODO
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_Rate])
    {
        lfo.setRate(newValue);     // TODO
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_Sync])
    {
        lfo.setSync(newValue);     //TODO
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_FadeInTime])
    {
        lfo.setFadeInTime(newValue);   // TODO
    }
    else if (parameterID == lfo_ParamIDs[kLfoParam_Delay])
    {
        lfo.setDelay(newValue);    // TODO
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

    osc.start(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    wtb.start(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    osc.stop();
    wtb.stop();
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
        double sample1 = osc.getNextSample() * level;
        double sample2 = wtb.getNextSample() * level;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample, sample1 + sample2);
        }
        ++startSample;
    }
}
