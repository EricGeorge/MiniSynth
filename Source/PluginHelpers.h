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

// calculates the bipolar (-1 -> +1) value from a unipolar (0 -> 1) value
inline double unipolarToBipolar(double value)
{
    return 2.0 * value - 1.0;
}
