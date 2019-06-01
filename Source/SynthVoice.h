/*
  ==============================================================================

    SineWaveVoice.h
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "BandLimitedOscillator.h"
#include "LowFrequencyOscillator.h"

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
    ~SynthVoice() override;
    
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
    
    void parameterChanged (const String& parameterID, float newValue);
    void osc1ParameterChanged (const String& parameterID, float newValue);
    void osc2ParameterChanged (const String& parameterID, float newValue);
    void lfo1ParameterChanged (const String& parameterID, float newValue);

private:
    
    float level;
    BLOsc osc1;
    BLOsc osc2;
    LFO lfo1;
};
