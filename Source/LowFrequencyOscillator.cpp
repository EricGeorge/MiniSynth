/*
  ==============================================================================

    LowFrequencyOscillator.cpp
    Created: 24 May 2019 9:35:45am
    Author:  Eric

  ==============================================================================
*/

#include "LowFrequencyOscillator.h"

#include "PluginHelpers.h"
#include "OscillatorParameters.h"

LowFrequencyOscillator::LowFrequencyOscillator(double sampleRate)
:   sampleRate(sampleRate),
    waveType(Sine),
    runState(Free),
    pulseWidth(0.5),
    phaseOffset(0.0),
    amount(1.0),
    polarityOffset(0.0),
    rate(1.0),
    sync(false),
    fadeInTime(0.0),
    delay(0.0)
{
    
}

LowFrequencyOscillator::~LowFrequencyOscillator()
{
    
}

void LowFrequencyOscillator::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<AudioParameterInt>(lfoWavetype_ParameterID, "Wavetype", 1, 5, 1));
    layout.add(std::make_unique<AudioParameterInt>(lfoRunState_ParameterID, "Run Type", 1, 5, 1));
    layout.add(std::make_unique<AudioParameterFloat>(lfoPulseWidth_ParameterID, "Pulse Width", NormalisableRange<float> (1.0f, 99.0f), 50.0f));
    layout.add(std::make_unique<AudioParameterFloat>(lfoPhaseOffset_ParameterID, "Phase Offset", NormalisableRange<float> (0.0f, 1.0f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>(lfoAmount_ParameterID, "Amount", NormalisableRange<float> (0.0f, 1.0f), 1.0f));
    layout.add(std::make_unique<AudioParameterFloat>(lfoPolarityOffset_ParameterID, "Polarity Offset", NormalisableRange<float> (-1.0f, 1.0f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>(lfoRate_ParameterID, "Rate", NormalisableRange<float> (1.0f, 20.0f), 1.0f));
    layout.add(std::make_unique<AudioParameterBool>(lfoSync_ParameterID, "Sync", false));
    layout.add(std::make_unique<AudioParameterFloat>(lfoFadeInTime_ParameterID, "Fade In Time", NormalisableRange<float> (0.0f, 1.0f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>(lfoDelay_ParameterID, "Delay", NormalisableRange<float> (0.0f, 1.0f), 0.0f));
}

void LowFrequencyOscillator::addParameterListeners(AudioProcessorValueTreeState& state)
{
    state.addParameterListener(lfoWavetype_ParameterID, this);
    state.addParameterListener(lfoRunState_ParameterID, this);
    state.addParameterListener(lfoPulseWidth_ParameterID, this);
    state.addParameterListener(lfoPhaseOffset_ParameterID, this);
    state.addParameterListener(lfoAmount_ParameterID, this);
    state.addParameterListener(lfoPolarityOffset_ParameterID, this);
    state.addParameterListener(lfoRate_ParameterID, this);
    state.addParameterListener(lfoSync_ParameterID, this);
    state.addParameterListener(lfoFadeInTime_ParameterID, this);
    state.addParameterListener(lfoDelay_ParameterID, this);
}

void LowFrequencyOscillator::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    state.removeParameterListener(lfoWavetype_ParameterID, this);
    state.removeParameterListener(lfoRunState_ParameterID, this);
    state.removeParameterListener(lfoPulseWidth_ParameterID, this);
    state.removeParameterListener(lfoPhaseOffset_ParameterID, this);
    state.removeParameterListener(lfoAmount_ParameterID, this);
    state.removeParameterListener(lfoPolarityOffset_ParameterID, this);
    state.removeParameterListener(lfoRate_ParameterID, this);
    state.removeParameterListener(lfoSync_ParameterID, this);
    state.removeParameterListener(lfoFadeInTime_ParameterID, this);
    state.removeParameterListener(lfoDelay_ParameterID, this);
}

void LowFrequencyOscillator::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == lfoWavetype_ParameterID)
    {
        waveType = static_cast<WaveType>(newValue);
    }
    else if (parameterID == lfoRunState_ParameterID)
    {
        runState = static_cast<RunState>(newValue); // TODO
    }
    else if (parameterID == lfoPulseWidth_ParameterID)
    {
        pulseWidth = newValue / 100.0f;
    }
    else if (parameterID == lfoPhaseOffset_ParameterID)
    {
        phaseOffset = newValue; // TODO
    }
    else if (parameterID == lfoAmount_ParameterID)
    {
        amount = newValue;
    }
    else if (parameterID == lfoPolarityOffset_ParameterID)
    {
        amount = newValue;  // TODO
    }
    else if (parameterID == lfoRate_ParameterID)
    {
        rate = newValue;    // TODO
    }
    else if (parameterID == lfoSync_ParameterID)
    {
        sync = static_cast<bool>(newValue); // TODO
    }
    else if (parameterID == lfoFadeInTime_ParameterID)
    {
        fadeInTime = newValue;  // TODO
    }
    else if (parameterID == lfoDelay_ParameterID)
    {
        delay = newValue;   // TODO
    }
}


void LowFrequencyOscillator::reset(double sampleRate)
{
    sampleRate = sampleRate;
    phaseAccumulator.reset(0.0, rate / sampleRate);
}

float LowFrequencyOscillator::getNextSample()
{
    float sample = 0.0f;
    
    switch (waveType)
    {
        case Saw:
            sample = getTrivialSawSample(phaseAccumulator.getPhase());
            break;
        case ReverseSaw:
            sample = getTrivialSawSample(phaseAccumulator.getPhase()) * -1;
            break;
        case Triangle:
            sample = getTrivialTriangleSample(phaseAccumulator.getPhase());
            break;
        case Pulse:
            sample = getTrivialPulseSample(phaseAccumulator.getPhase(), pulseWidth / 100.0);
            break;
        case Sine:
            sample = (float)(parabolicSine(phaseAccumulator.getPhase() * 2.0 * pi - pi));
            break;
    }
    
    phaseAccumulator.IncrementPhase();
    
    return sample * amount;
}
