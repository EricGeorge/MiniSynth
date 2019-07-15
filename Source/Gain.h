/*
  ==============================================================================

    Gain.h
    Created: 21 May 2019 10:33:15am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Gain
{
public:
    Gain();
    ~Gain();
    
    void setGain(float newValue);
    
    void process(juce::AudioBuffer<float>& buffer,
                 int inNumSamplesToRender);
    
private:
    // parameters
    double gain;
    double gainMapped;
    double gainSmoothed;
    
    void mapGain();
};
