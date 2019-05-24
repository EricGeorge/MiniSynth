/*
  ==============================================================================

    BlepOscillator.h
    Created: 24 May 2019 9:34:57am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class BandLimitedOscillator : public AudioProcessorValueTreeState::Listener
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
    
    static void createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout);
    void addParameterListeners(AudioProcessorValueTreeState& state);
    void removeParameterListeners(AudioProcessorValueTreeState& state);
    
    void parameterChanged (const String& parameterID, float newValue) override;

    void reset(double sampleRate);
    
    void startNote(double frequency);
    void stopNote();
    
    float getNextSample();
    
private:
    double sampleRate;
    
    double phaseInc;
    double phase;
    bool phaseReset;
    double modFrequency;
    
    double dpwSquareModulator;
    double dpwRegister;
    unsigned int seed;
    
    // parameters
    WaveType waveType;
    int octaves;
    int semitones;
    int cents;
    float pulseWidth;
    float polyBLEPMix;
    float waveShapeSaturation;
    
    // poor man's envelope - replace with ADSR
    bool noteOn;
    
    float getNextPolyBLEPSawSquareModSample();
    float getNextBLEPSawSample();
    float getNextUnipolarWaveShapedSawSample();
    float getNextBipolarWaveShapedSawSample();
    float getNextSumOfSawSquarePWMSample();
    float getNextDPWTriangleSample();
    float getNextParabolicSineSample();
    float getNextWhiteNoiseSample();
    float getNextRandomNoiseSample();
};

using BLOsc = BandLimitedOscillator;
