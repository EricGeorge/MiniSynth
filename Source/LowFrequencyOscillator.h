/*
  ==============================================================================

    LowFrequencyOscillator.h
    Created: 24 May 2019 9:35:45am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "OscillatorHelpers.h"

class LowFrequencyOscillator : public AudioProcessorValueTreeState::Listener
{
public:
    LowFrequencyOscillator(double sampleRate);
    ~LowFrequencyOscillator();
    
    enum WaveType
    {
        Saw = 1,
        ReverseSaw,
        Triangle,
        Pulse,
        Sine
    };
    
    enum RunState
    {
        Free,
        Sync,
        OneShot
    };
    
    static void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);
    void parameterChanged (const String& parameterID, float newValue) override;

    void reset(double sampleRate);

    float getNextSample();
    
private:
    double sampleRate;
    
    PhaseAccumulator phaseAccumulator;
    
    // parameters
    WaveType waveType;
    RunState runState;
    double pulseWidth;
    double phaseOffset;
    double amount;
    double polarityOffset;
    double rate;
    bool sync;
    double fadeInTime;
    double delay;
    
};

using LFO = LowFrequencyOscillator;
