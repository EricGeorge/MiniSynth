/*
  ==============================================================================

    WavetableOscillator.h
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#pragma once

class WaveTableOscillator
{
    WaveTableOscillator();
    ~WaveTableOscillator();
    
    float getNextSample();
};

using WTOsc = WaveTableOscillator;
