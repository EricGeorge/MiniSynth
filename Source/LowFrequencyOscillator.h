/*
  ==============================================================================

    LowFrequencyOscillator.h
    Created: 24 May 2019 9:35:45am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "OscillatorHelpers.h"

class LowFrequencyOscillator
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
    
    void reset(double sampleRate);
    double getNextSample();
    
    // parameter setters
    void setWaveType(float newValue);
    void setRunState(float newValue);
    void setPulseWidth(float newValue);
    void setPhaseOffset(float newValue);
    void setAmount(float newValue);
    void setPolarityOffset(float newValue);
    void setRate(float newValue);
    void setSync(float newValue);
    void setFadeInTime(float newValue);
    void setDelay(float newValue);

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
