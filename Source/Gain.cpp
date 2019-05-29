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
:   gain(Decibels::decibelsToGain(-3.0f)),
    gainSmoothed(gain)
{
}

Gain::~Gain()
{
}

void Gain::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<AudioParameterFloat>(outputGain_ParameterID, "Output Gain", NormalisableRange<float> (0.0f, 1.0f), Decibels::decibelsToGain(-3.0f)));
}

void Gain::addParameterListeners(AudioProcessorValueTreeState& state)
{
    state.addParameterListener(outputGain_ParameterID, this);
}

void Gain::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    state.removeParameterListener(outputGain_ParameterID, this);
}

void Gain::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == outputGain_ParameterID)
    {
        gain = newValue;
    }
}

void Gain::process(AudioBuffer<float>& buffer, int numSamplesToProcess)
{
    double gainMapped = (double)jmap(gain, 0.0f, 1.0f, -24.0f, 0.0f);
    gainMapped = Decibels::decibelsToGain(gainMapped, kMinimumDecibels);
    
    for (int index = 0; index < numSamplesToProcess; ++index)
    {
        gainSmoothed = gainSmoothed - kParameterSmoothingCoeff_Fine * (gainSmoothed - gainMapped);
        
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float processedSample = buffer.getSample(channel, index) * gainSmoothed;
            buffer.setSample(channel, index, processedSample);
        }
    }
}
