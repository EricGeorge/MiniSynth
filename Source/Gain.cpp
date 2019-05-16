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
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sampleIndex = 0; sampleIndex < numSamplesToProcess; ++sampleIndex)
        {
            gainSmoothed = gainSmoothed - kParameterSmoothingCoeff_Fine * (gainSmoothed - gainMapped);

            float processedSample = buffer.getSample(channel, sampleIndex) * gainSmoothed;
            buffer.setSample(channel, sampleIndex, processedSample);
        }
    }
}
