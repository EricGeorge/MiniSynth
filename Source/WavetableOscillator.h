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

class WavetableOscillator
{
public:
    WavetableOscillator(double sampleRate);
    ~WavetableOscillator();
    
    float getNextSample();
    
    void reset(double sampleRate);
    void update();
    
    void start(double frequency);
    void stop();


    // parameter setters
    void setPosition(float newValue);
    void setInterpolate(float newValue);
    void setSemitones(float newValue);
    void setCents(float newValue);
    void setVolume(float newValue);

private:
    double sampleRate;
    double frequency;
    
    PhaseAccumulator phaseAccumulator;

    // parameters
    float position;
    bool interpolate;
    int semitones;
    double cents;
    double volume;
    
    // poor man's envelope - replace with ADSR
    bool noteOn;

    int currentWaveform;      // current table, based on current frequency
    Wavetable wavetable;
};

using WTOsc = WavetableOscillator;



