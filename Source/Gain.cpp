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
:   gain(outputGainInitialValue),
    gainMapped(gain),
    gainSmoothed(gain)
{
    mapGain();
}

Gain::~Gain()
{
}

void Gain::setGain(float newValue)
{
    gain = newValue;
    mapGain();
}

void Gain::mapGain()
{
    double gainInDB = juce::jmap(gain, 0.0, 1.0, kMinimumDecibels, kMaximumDecibels);
    gainMapped = juce::Decibels::decibelsToGain(gainInDB, kMinimumDecibels);
}

void Gain::process(juce::AudioBuffer<float>& buffer, int numSamplesToProcess)
{
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
