/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthVoice.h"

SynthVoice::SynthVoice()
:   osc(getSampleRate())
{
}

SynthVoice::~SynthVoice()
{
}

bool SynthVoice::canPlaySound(SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity,
                           SynthesiserSound* sound, int currentPitchWheelPosition)
{
    // will be replaced with a proper level control
    level = velocity * 0.3;
    
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
    while (--numSamples >= 0)
    {
        float sample = osc.getNextSample() * level;
        
        outputBuffer.addSample(0, startSample, sample);
        outputBuffer.addSample(1, startSample, sample);
        ++startSample;
    }
}
