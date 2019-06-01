/*
  ==============================================================================

    Gain.cpp
    Created: 21 May 2019 10:33:15am
    Author:  Eric

  ==============================================================================
*/

#include "Gain.h"

#include "OutputParameters.h"
#include "PluginHelpers.h"

Gain::Gain()
:   gain(juce::Decibels::decibelsToGain(-3.0f)),
    gainSmoothed(gain)
{
}

Gain::~Gain()
{
}

void Gain::process(juce::AudioBuffer<float>& buffer, int numSamplesToProcess)
{
    double gainMapped = juce::jmap(gain, 0.0, 1.0, -24.0, 0.0);
    gainMapped = juce::Decibels::decibelsToGain(gainMapped, kMinimumDecibels);
    
    for (int index = 0; index < numSamplesToProcess; ++index)
    {
        gainSmoothed = gainSmoothed - kParameterSmoothingCoeff_Fine * (gainSmoothed - gainMapped);
        
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            double processedSample = buffer.getSample(channel, index) * gainSmoothed;
            buffer.setSample(channel, index, processedSample);
        }
    }
}
