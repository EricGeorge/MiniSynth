/*
  ==============================================================================

    Gain.h
    Created: 21 May 2019 10:33:15am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Gain : public AudioProcessorValueTreeState::Listener
{
public:
    Gain();
    ~Gain();
    
    static void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);
    
    void parameterChanged (const String& parameterID, float newValue) override;
    
    void process(AudioBuffer<float>& buffer,
                 int inNumSamplesToRender);
    
private:
    // parameters
    float gain;
    
    float gainSmoothed;
};
