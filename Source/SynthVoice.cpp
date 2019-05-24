/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthVoice.h"

SynthVoice::SynthVoice()
:   level(0.0f),
    osc(getSampleRate())
{
}

SynthVoice::~SynthVoice()
{
}

void SynthVoice::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    BLOsc::createParameterLayout(layout);
}

void SynthVoice::addParameterListeners(AudioProcessorValueTreeState& state)
{
    osc.addParameterListeners(state);
}

void SynthVoice::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    osc.removeParameterListeners(state);
}

bool SynthVoice::canPlaySound(SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity,
                           SynthesiserSound* sound, int currentPitchWheelPosition)
{
    level = velocity * 0.25;

    osc.startNote(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    osc.stopNote();
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
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        while (--numSamples >= 0)
        {
            float sample = osc.getNextSample() * level;
            outputBuffer.addSample(channel, startSample, sample);
            ++startSample;
        }
    }
}
