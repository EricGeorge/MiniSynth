/*
  ==============================================================================

    EnvelopeHelpers.h
    Created: 12 Jul 2019 12:49:32pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cmath>

inline double calculateEnvCoefficient(double maxLevel, double timeConstant, double numSamples)
{
    return std::exp(-std::log((maxLevel + timeConstant) / timeConstant) / numSamples);
}

inline double calculateEnvOffset(double maxLevel, double timeConstant, double coefficient)
{
    return (maxLevel + timeConstant) * (1.0 - coefficient);
}
