/*
  ==============================================================================

    SineWaveVoice.h
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrivialOscillator.h"

struct SynthSound : public SynthesiserSound
{
    SynthSound() {}
    
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

struct SynthVoice : public SynthesiserVoice
{
    SynthVoice();
    ~SynthVoice();
    
    bool canPlaySound(SynthesiserSound* sound) override;
    
    void startNote(int midiNoteNumber,
                   float velocity,
                   SynthesiserSound* sound,
                   int currentPitchWheelPosition) override;
    
    void stopNote(float velocity,
                  bool allowTailOff) override;
    
    void pitchWheelMoved(int newPitchwheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    
private:
    double level = 0.0;
    
    TrivialOscillator osc;
};
