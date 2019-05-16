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

class SynthSound : public SynthesiserSound
{
public:
    SynthSound() {}
    
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

class SynthVoice : public SynthesiserVoice
{
public:
    SynthVoice();
    ~SynthVoice();
    
    static void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);

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
    
    float level;
    TrivialOscillator osc;
};
