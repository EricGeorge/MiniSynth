/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthVoice.h"

#include "AmplifierParameters.h"
#include "EnvelopeParameters.h"
#include "OscillatorParameters.h"
#include "WavetableParameters.h"

#include "SynthSound.h"

SynthVoice::SynthVoice(Synth& synth)
:   synth(synth),
    level(0.0f),
    wtb(getSampleRate(), synth.getSynthSound()),
    env(getSampleRate()),
    amp()
{
}

SynthVoice::~SynthVoice()
{
}

void SynthVoice::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID.contains(wavetableParamIDPrefix))
    {
        wtbParameterChanged(parameterID, newValue);
    }
    else if (parameterID.contains(envelopeParamIDPrefix))
    {
        envParameterChanged(parameterID, newValue);
    }
    else if (parameterID.contains(amplifierParamIDPrefix))
    {
        ampParameterChanged(parameterID, newValue);
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

void SynthVoice::envParameterChanged(const String &parameterID, float newValue)
{
    if (parameterID == envelope_ParamIDs[kEnvParam_Attack])
    {
        env.setAttack(newValue);
    }
    else if (parameterID == envelope_ParamIDs[kEnvParam_Decay])
    {
        env.setDecay(newValue);
    }
    else if (parameterID == envelope_ParamIDs[kEnvParam_Sustain])
    {
        env.setSustain(newValue);
    }
    else if (parameterID == envelope_ParamIDs[kEnvParam_Release])
    {
        env.setRelease(newValue);
    }
}

void SynthVoice::ampParameterChanged(const String &parameterID, float newValue)
{
    if (parameterID == amplifier_ParamIDs[kAmpParam_Gain])
    {
        amp.setGain(newValue);
    }
    else if (parameterID == amplifier_ParamIDs[kAmpParam_Pan])
    {
        amp.setPan(newValue);
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

    wtb.start(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
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
        double sample = wtb.getNextSample() * level;
        double envMod = env.getNextSampleMod();
        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            double processedSample = amp.processSample(sample, channel, envMod);
            outputBuffer.addSample(channel, startSample, processedSample);
        }
        ++startSample;
    }
}
