/*
  ==============================================================================

    PluginHelpers.h
    Created: 13 May 2019 8:09:10pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

const int kNumVoices = 16;
const float kParameterSmoothingCoeff_Fine = 0.005f;
const double kMinimumDecibels = -24.0;

inline double pitchShiftMultiplier(double semitonesToShift)
{
    // 2^(N/12)
    return pow(2.0, semitonesToShift / 12.0);
}

// returns the frequency multiplier for the pitch change
inline double getPitchFreqMod(float octaves, float semitones, float cents)
{
    return pitchShiftMultiplier(octaves * 12 + semitones + cents / 100.0f);
}

// calculates the bipolar (-1 -> +1) value from a unipolar (0 -> 1) value
inline double unipolarToBipolar(double value)
{
    return 2.0 * value - 1.0;
}

inline double tanh_clip(double x)
{
    return x * (27 + x * x) / (27 + 9 * x * x);
}

inline float linear_interp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

