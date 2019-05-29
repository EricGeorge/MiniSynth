/*
  ==============================================================================

    OscillatorHelpers.h
    Created: 24 May 2019 9:36:41am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

const double pi = MathConstants<double>::pi;

//    Taken from Will Pirkle's Designing Software Synthesizers in C++
//    book.  This polynomial was derived by taking a unipolar triangle pulse
//    (which is the linear approximation of a windowed sinc() pulse) and
//    integrating it.  This results in:
//
//    sn(t) = 0; t < -1
//    sn(t) = t^2/2 + t + 1/2; -1 <= t <= 0
//    sn(t) = t - t^2/2 + 1/2; 0 < t <= 1
//    sn(t) = 1; t > 1
//
//    which simplifies to:
//
//    t^2 + 2t + 1;  -1 <= t <= 0
//    2t - t - 1; 0 < t <= 1

inline float polyBLEP(float t, float phaseInc)
{
    if (t < phaseInc)
    {
        t = t / phaseInc;
        return (t + t) - (t * t) - 1.0f;
    }

    if (t > (1.0f - phaseInc))
    {
        t = (t - 1.0f) / phaseInc;
        return (t * t) + (t + t) + 1.0f;
    }

    return 0;
}


static const double B = 4.0 / pi;
static const double C = -4.0 / (pi * pi);
static const double P = 0.225;

// input is -pi to +pi
inline double parabolicSine(double x, bool highPrecision = true)
{
    double y = B * x + C * x * fabs(x);
    
    if(highPrecision)
        y = P * (y * fabs(y) - y) + y;
    
    return y;
}

const double D = 5.0 * pi * pi;
inline double BhaskaraISine(double x)
{
    double d = fabs(x);
    double sgn = d == 0 ? 1.0 : x / fabs(x);
    return 16.0 * x * (pi - sgn * x) / (D - sgn * 4.0 * x * (pi - sgn * x));
}

inline unsigned int extractBits(unsigned int value, unsigned int start, unsigned int length)
{
    return (value >> (start - 1)) & ((1 << length) - 1);
}

// calculates the bipolar (-1 -> +1) value from a unipolar (0 -> 1) value
inline double unipolarToBipolar(double value)
{
    return 2.0 * value - 1.0;
}

inline double getTrivialSawSample(double phase)
{
    return unipolarToBipolar(phase);
}

inline double getTrivialPulseSample(double phase, double pulseWidth)
{
    return phase > pulseWidth ? -1.0 : 1.0;
}

inline double getTrivialTriangleSample(double phase)
{
    return 2.0f * fabs(2.0f * phase - 1.0f) - 1.0f;
}

class PhaseAccumulator
{
public:
    PhaseAccumulator()
    :   phase(0.0),
        phaseInc(0.0) {};
    
    void IncrementPhase()
    {
        phase += phaseInc;
        
        if (phase > 1.0)
            phase -= 1.0;
    }
    
    double getPhase()
    {
        return phase;
    }
    
    double getPhaseInc()
    {
        return phaseInc;
    }
    
    void reset(double inPhase, double inPhaseInc)
    {
        phase = inPhase;
        phaseInc = inPhaseInc;
    }
    
private:
    double phase;
    double phaseInc;
};
