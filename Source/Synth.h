/*
  ==============================================================================

    Synth.h
    Created: 20 May 2019 10:21:21am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Gain.h"

class Synth :   public Synthesiser,
                public AudioProcessorValueTreeState::Listener
{
public:
    Synth();
    ~Synth() override;
    
    void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);
    void parameterChanged (const String& parameterID, float newValue) override;

    void renderNextBlock (AudioBuffer<float>& outputAudio,
                          const MidiBuffer& inputMidi,
                          int startSample,
                          int numSamples);

private:
    // synth components external to the voice
    Gain outputGain;
};
