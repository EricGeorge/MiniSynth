/*
  ==============================================================================

    PluginHelpers.h
    Created: 13 May 2019 8:09:10pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <vector>

const int kNumVoices = 1;
const double kParameterSmoothingCoeff_Fine = 0.005f;
const double kMinimumDecibels = -24.0;
const double kMaximumDecibels = 0.0;

inline double getPitchFreqMod(double semitonesToShift)
{
    // 2^(N/12)
    return pow(2.0, semitonesToShift / 12.0);
}

inline double tanh_clip(double x)
{
    return x * (27.0 + x * x) / (27.0 + 9.0 * x * x);
}

inline float linear_interp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

/** These methods skew a range around an anchor point - helpful in making a parameter have a range that is more detailed on one end
 */
// value = start + (end - start) * e^(ln(normalizedValue) / ln(anchorPoint) / ln((anchorPointValue - start) / (end - start))
inline float convertToRangeWithAnchor(double start, double end, double normalizedValue, double anchorPoint, double anchorPointValue)
{
    double skew = std::log(anchorPoint) / std::log((anchorPointValue - start) / (end - start));
    double proportion = std::exp(std::log(normalizedValue) / skew);
    double value = start + (end - start) * proportion;
    
    return value;
}

// normlizedValue = e^ln((value - start) / (end - start)) * ln(anchorPoint) / ln((anchorPointValue - start) / (end - start))
inline float convertFromRangeWithAnchor(double start, double end, double value, double anchorPoint, double anchorPointValue)
{
    double skew = std::log(anchorPoint) / std::log((anchorPointValue - start) / (end - start));
    double proportion = std::log((value - start) / (end - start));
    double normalizedValue = std::exp(proportion * skew);
    
    return normalizedValue;
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
