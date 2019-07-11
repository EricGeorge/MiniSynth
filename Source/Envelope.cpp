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
    attackCoefficient(0.0),
    attackOffset(0.0),
    decayCoefficient(0.0),
    decayOffset(0.0),
    releaseCoefficient(0.0),
    releaseOffset(0.0)
{
    attackTCO = exp(-1.5);
    decayTCO = exp(-4.95);
    releaseTCO = decayTCO;
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
    
    attackCoefficient = std::exp(-std::log((1.0 + attackTCO) / attackTCO) / numSamples);
    attackOffset = (1.0 + attackTCO) * (1.0 - attackCoefficient);
}

void Envelope::setDecay(double newValue)
{
    decayRate = newValue;
    
    double numSamples = decayRate / 1000.0 * sampleRate;
    
    decayCoefficient = std::exp(-std::log((1.0 + decayTCO) / decayTCO) / numSamples);
    decayOffset = (sustainLevel  - decayTCO) * (1.0 - decayCoefficient);
}

void Envelope::setSustain(double newValue)
{
    sustainLevel = newValue;
}

void Envelope::setRelease(double newValue)
{
    releaseRate = newValue;
    
    double numSamples = releaseRate / 1000.0 * sampleRate;
    
    releaseCoefficient = std::exp(-std::log((1.0 + releaseTCO) / releaseTCO) / numSamples);
    releaseOffset = -releaseTCO * (1.0 - releaseCoefficient);
}

Envelope::State Envelope::getState() const
{
    return state;
}
