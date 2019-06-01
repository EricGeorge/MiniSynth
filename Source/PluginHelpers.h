/*
  ==============================================================================

    PluginHelpers.h
    Created: 13 May 2019 8:09:10pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cmath>

const int kNumVoices = 16;
const double kParameterSmoothingCoeff_Fine = 0.005f;
const double kMinimumDecibels = -24.0;

inline double pitchShiftMultiplier(double semitonesToShift)
{
    // 2^(N/12)
    return pow(2.0, semitonesToShift / 12.0);
}

// returns the frequency multiplier for the pitch change
inline double getPitchFreqMod(double octaves, double semitones, double cents)
{
    return pitchShiftMultiplier(octaves * 12 + semitones + cents / 100.0);
}

inline double tanh_clip(double x)
{
    return x * (27.0 + x * x) / (27.0 + 9.0 * x * x);
}

inline float linear_interp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

class Differentiator
{
public:
    Differentiator(double value)
    :   priorValue(value) {}
    
    double process(double value)
    {
        double  result = value - priorValue;
        priorValue = value;
        return result;
    }
    
    void reset(double value) { priorValue = value; }
    
private:
    double priorValue;
};

