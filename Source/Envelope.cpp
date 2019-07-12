/*
  ==============================================================================

    Envelope.cpp
    Created: 6 Jul 2019 5:30:04pm
    Author:  Eric

  ==============================================================================
*/

#include "Envelope.h"

#include "EnvelopeHelpers.h"
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
    
    updateAttackCalculations();
}

void Envelope::setDecay(double newValue)
{
    decayRate = newValue;
    
    updateDecayCalculations();
}

void Envelope::setSustain(double newValue)
{
    sustainLevel = newValue;
    
    updateDecayCalculations();
}

void Envelope::setRelease(double newValue)
{
    releaseRate = newValue;
    
    updateReleaseCalculations();
}

void Envelope::setAttackCurve(double newValue)
{
    attackCurve = newValue;
    
    updateAttackCalculations();
}

void Envelope::setDecayCurve(double newValue)
{
    decayCurve = newValue;
    
    updateDecayCalculations();
}

void Envelope::setReleaseCurve(double newValue)
{
    releaseCurve = newValue;
    
    updateReleaseCalculations();
}

Envelope::State Envelope::getState() const
{
    return state;
}

void Envelope::updateAttackCalculations()
{
    attackCoefficient = calculateEnvCoefficient(1.0, attackCurve, attackRate / 1000.0 * sampleRate);
    attackOffset = calculateEnvOffset(1.0, attackCurve, attackCoefficient);
}

void Envelope::updateDecayCalculations()
{
    decayCoefficient = calculateEnvCoefficient(1.0 - sustainLevel, decayCurve, decayRate / 1000.0 * sampleRate);
    decayOffset = calculateEnvOffset(sustainLevel, -decayCurve, decayCoefficient);
}

void Envelope::updateReleaseCalculations()
{
    releaseCoefficient = calculateEnvCoefficient(sustainLevel, releaseCurve, releaseRate / 1000.0 * sampleRate);
    releaseOffset = calculateEnvOffset(0.0, -releaseCurve, releaseCoefficient);
}

