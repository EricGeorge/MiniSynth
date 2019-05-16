/*
  ==============================================================================

    Synth.h
    Created: 20 May 2019 10:21:21am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Synth : public Synthesiser
{
public:
    Synth();
    ~Synth();
    
    static void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);
    
private:
    
};
