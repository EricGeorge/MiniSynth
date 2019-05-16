/*
  ==============================================================================

    NaiveOscillator.h
    Created: 13 May 2019 8:15:21pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum Waveform {
    Waveform_Sine = 1,
    Waveform_Saw,
    Waveform_Triangle,
    Waveform_Pulse
};

class TrivialOscillator : public AudioProcessorValueTreeState::Listener
{
public:
    TrivialOscillator(double sampleRate);
    ~TrivialOscillator();
    
    static void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);

    void parameterChanged (const String& parameterID, float newValue) override;
    
    void reset(double sampleRate);
    
    void startNote(double frequency);
    void stopNote();
    
    float getNextSample();
    
private:
    double sampleRate;
    
    double phaseInc;
    double phase;
    
    // parameters
    Waveform waveform;
    int octaves;
    int semitones;
    int cents;
    float pulseWidth;

    // poor man's envelope - replace with ADSR
    bool noteOn;
};
