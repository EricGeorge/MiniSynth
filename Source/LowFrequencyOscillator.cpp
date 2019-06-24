/*
  ==============================================================================

    LowFrequencyOscillator.cpp
    Created: 24 May 2019 9:35:45am
    Author:  Eric

  ==============================================================================
*/

#include "LowFrequencyOscillator.h"

#include "PluginHelpers.h"

LowFrequencyOscillator::LowFrequencyOscillator(double sampleRate)
:   sampleRate(sampleRate),
    waveType(Sine),
    runState(Free),
    pulseWidth(0.5),
    phaseOffset(0.0),
    amount(1.0),
    polarityOffset(0.0),
    rate(1.0),
    sync(false),
    fadeInTime(0.0),
    delay(0.0)
{
    
}

LowFrequencyOscillator::~LowFrequencyOscillator()
{
    
}

void LowFrequencyOscillator::setWaveType(float newValue)
{
    waveType = static_cast<WaveType>(newValue);
}

void LowFrequencyOscillator::setRunState(float newValue)
{
    runState = static_cast<RunState>(newValue);
}

void LowFrequencyOscillator::setPulseWidth(float newValue)
{
    pulseWidth = newValue / 100.0;
}

void LowFrequencyOscillator::setPhaseOffset(float newValue)
{
    phaseOffset = newValue;
}

void LowFrequencyOscillator::setAmount(float newValue)
{
    amount = newValue;
}

void LowFrequencyOscillator::setPolarityOffset(float newValue)
{
    polarityOffset = newValue;
}

void LowFrequencyOscillator::setRate(float newValue)
{
    rate = newValue;
}

void LowFrequencyOscillator::setSync(float newValue)
{
    sync = newValue;
}

void LowFrequencyOscillator::setFadeInTime(float newValue)
{
    fadeInTime = newValue;
}

void LowFrequencyOscillator::setDelay(float newValue)
{
    delay = newValue;
}

void LowFrequencyOscillator::reset(double sampleRate)
{
    sampleRate = sampleRate;
    phaseAccumulator.reset(rate / sampleRate);
}

double LowFrequencyOscillator::getNextSample()
{
    double sample = 0.0;
    
    switch (waveType)
    {
        case Saw:
            sample = getTrivialSawSample(phaseAccumulator.getPhase());
            break;
        case ReverseSaw:
            sample = getTrivialSawSample(phaseAccumulator.getPhase()) * -1;
            break;
        case Triangle:
            sample = getTrivialTriangleSample(phaseAccumulator.getPhase());
            break;
        case Pulse:
            sample = getTrivialPulseSample(phaseAccumulator.getPhase(), pulseWidth / 100.0);
            break;
        case Sine:
            sample = (float)(parabolicSine(phaseAccumulator.getPhase() * 2.0 * pi - pi));
            break;
    }
    
    phaseAccumulator.IncrementPhase();
    
    return sample * amount;
}
