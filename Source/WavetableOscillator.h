/*
  ==============================================================================

    WavetableOscillator.h
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "OscillatorHelpers.h"
#include "Wavetable.h"

class WaveTableOscillator
{
public:
    WaveTableOscillator(double sampleRate);
    ~WaveTableOscillator();
    
    float getNextSample();
    
    void reset(double sampleRate);
    
    void start(double frequency);
    void stop();


    // parameter setters
    void setOctaves(float newValue);
    void setSemitones(float newValue);
    void setCents(float newValue);
    void setVolume(float newValue);

private:
    double sampleRate;
    PhaseAccumulator phaseAccumulator;

    // parameters
    int octaves;
    int semitones;
    int cents;
    double volume;
    
    // poor man's envelope - replace with ADSR
    bool noteOn;

    int currentWaveform;      // current table, based on current frequency
    Wavetable wavetable;
};

using WTOsc = WaveTableOscillator;



