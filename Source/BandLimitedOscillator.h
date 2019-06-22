/*
  ==============================================================================

    BlepOscillator.h
    Created: 24 May 2019 9:34:57am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "PluginHelpers.h"
#include "OscillatorHelpers.h"

class BandLimitedOscillator
{
public:
    BandLimitedOscillator(double sampleRate);
    ~BandLimitedOscillator();
    
    enum WaveType
    {
        BLEPSaw = 1,
        PolyBLEPSawSquareMod,
        UnipolarWaveShapedSaw,
        BipolarWaveShapedSaw,
        SumofSawSquarePWM,
        DPWTriangle,
        ParabolicSine,
        WhiteNoise,
        RandomNoise
    };
    

    void reset(double sampleRate);
    void update();
    
    void start(double frequency);
    void stop();
    
    double getNextSample();
    
    // parameter setters
    void setWaveType(float newValue);
    void setOctaves(float newValue);
    void setSemitones(float newValue);
    void setCents(float newValue);
    void setPulseWidth(float newValue);
    void setPolyBLEPMix(float newValue);
    void setWaveShapeSaturation(float newValue);
    void setVolume(float newValue);
    
private:
    double sampleRate;
    double frequency;
    
    PhaseAccumulator phaseAccumulator;

    // for Differentiated Parabolic Waveform
    PhaseAccumulator dpwPhaseAccumulator;
    Differentiator dpwDifferentiator;
    
    // for random based oscillators
    unsigned int seed;
    
    // parameters
    WaveType waveType;
    int octaves;
    int semitones;
    int cents;
    double pulseWidth;
    double polyBLEPMix;
    double waveShapeSaturation;
    double volume;
    
    // poor man's envelope - replace with ADSR
    bool noteOn;
    
    double getNextPolyBLEPSawSquareModSample();
    double getNextBLEPSawSample();
    double getNextUnipolarWaveShapedSawSample();
    double getNextBipolarWaveShapedSawSample();
    double getNextSumOfSawSquarePWMSample();
    double getNextDPWTriangleSample();
    double getNextParabolicSineSample();
    double getNextWhiteNoiseSample();
    double getNextRandomNoiseSample();    
};

using BLOsc = BandLimitedOscillator;
