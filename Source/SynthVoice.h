/*
  ==============================================================================

    SineWaveVoice.h
    Created: 11 May 2019 9:07:54pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Amplifier.h"
#include "BandLimitedOscillator.h"
#include "Envelope.h"
#include "LowFrequencyOscillator.h"
#include "Synth.h"
#include "WavetableOscillator.h"

class SynthVoice : public SynthesiserVoice
{
public:
    SynthVoice(Synth& synth);
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
    
    void parameterChanged(const String& parameterID, float newValue);
    void oscParameterChanged(const String& parameterID, float newValue);
    void wtbParameterChanged(const String& parameterID, float newValue);
    void lfoParameterChanged(const String& parameterID, float newValue);
    void envParameterChanged(const String& parameterID, float newValue);
    void ampParameterChanged(const String& parameterID, float newValue);

private:
    Synth& synth;
    float level;
    BLOsc osc;
    WTOsc wtb;
    Env env;
    Amp amp;
    LFO lfo;
};
