/*
  ==============================================================================

    Envelope.cpp
    Created: 6 Jul 2019 5:30:04pm
    Author:  Eric

  ==============================================================================
*/

#include "Envelope.h"

#include "EnvelopeParameters.h"

Envelope::Envelope(double sampleRate)
:   sampleRate(sampleRate),
    state(Idle),
    output(0.0),
    attackRate(envAttackInitialValue),
    decayRate(envDecayInitialValue),
    sustainLevel(envSustainInitialValue),
    releaseRate(envReleaseInitialValue),
    attackCurve(envAttackCurveInitialValue),
    decayCurve(envDecayCurveInitialValue),
    releaseCurve(envReleaseCurveInitialValue),
    attackCoefficient(0.0),
    attackOffset(0.0),
    decayCoefficient(0.0),
    decayOffset(0.0),
    releaseCoefficient(0.0),
    releaseOffset(0.0)
{
}

Envelope::~Envelope()
{
    
}

void Envelope::start()
{
    state = Attack;
}

void Envelope::end()
{
    if (state != Idle)
    {
        state = Release;
    }
}

double Envelope::getNextSampleMod()
{    
    switch (state)
    {
        case Idle:
            output = 0.0;
            break;
        case Attack:
            output = attackOffset + output * attackCoefficient;
            if (output >= 1.0)
            {
                output = 1.0;
                state = Decay;
            }
            
            break;
        case Decay:
            output = decayOffset + output * decayCoefficient;
            if (output <= sustainLevel)
            {
                output = sustainLevel;
                state = Sustain;
            }
            break;
        case Sustain:
            output = sustainLevel;
            
            break;
        case Release:
            output = releaseOffset + output * releaseCoefficient;
            if (output <= 0.0)
            {
                output = 0.0;
                state = Idle;
            }
            break;
    }
    
    return output;
}

void Envelope::setAttack(double newValue)
{
    attackRate = newValue;
    
    double numSamples = attackRate / 1000.0 * sampleRate;
    
    attackCoefficient = std::exp(-std::log((1.0 + attackCurve) / attackCurve) / numSamples);
    attackOffset = (1.0 + attackCurve) * (1.0 - attackCoefficient);
}

void Envelope::setDecay(double newValue)
{
    decayRate = newValue;
    
    double numSamples = decayRate / 1000.0 * sampleRate;
    
    decayCoefficient = std::exp(-std::log((1.0 + decayCurve) / decayCurve) / numSamples);
    decayOffset = (sustainLevel  - decayCurve) * (1.0 - decayCoefficient);
}

void Envelope::setSustain(double newValue)
{
    sustainLevel = newValue;
}

void Envelope::setRelease(double newValue)
{
    releaseRate = newValue;
    
    double numSamples = releaseRate / 1000.0 * sampleRate;
    
    releaseCoefficient = std::exp(-std::log((1.0 + releaseCurve) / releaseCurve) / numSamples);
    releaseOffset = -releaseCurve * (1.0 - releaseCoefficient);
}

void Envelope::setAttackCurve(double newValue)
{
    attackCurve = newValue;
}

void Envelope::setDecayCurve(double newValue)
{
    decayCurve = newValue;
}

void Envelope::setReleaseCurve(double newValue)
{
    releaseCurve = newValue;
}

Envelope::State Envelope::getState() const
{
    return state;
}
