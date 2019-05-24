/*
  ==============================================================================

    LowFrequencyOscillator.h
    Created: 24 May 2019 9:35:45am
    Author:  Eric

  ==============================================================================
*/

#pragma once

class LowFrequencyOscillator
{
    LowFrequencyOscillator();
    ~LowFrequencyOscillator();
    
    enum WaveType
    {
        Saw,
        ReverseSaw,
        Triangle,
        Square,
        Sine,
        Exponential,
        SampleHold,
        SampleHold2
    };
    
    float getNextSample();
};

using LFO = LowFrequencyOscillator;
