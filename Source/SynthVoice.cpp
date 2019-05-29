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
    osc1(getSampleRate()),
    osc2(getSampleRate())
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
    osc1.addParameterListeners(state);
    osc2.addParameterListeners(state);
}

void SynthVoice::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    osc1.removeParameterListeners(state);
    osc2.removeParameterListeners(state);
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
        float sample1 = osc1.getNextSample() * level;
        float sample2 = osc2.getNextSample() * level;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample, sample1 * 0.5 + sample2 * 0.5);
        }
        ++startSample;
    }
}
