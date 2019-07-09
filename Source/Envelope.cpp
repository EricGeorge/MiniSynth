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
    attack(envAttackInitialValue),
    decay(envDecayInitialValue),
    sustain(envSustainInitialValue),
    release(envReleaseInitialValue),
    attackIncrement(0.0),
    decayIncrement(0.0),
    releaseIncrement(0.0)
{
    update();
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
            break;
        case Attack:
            output += attackIncrement;
            if (output >= 1.0)
            {
                state = Decay;
            }
            break;
        case Decay:
            output -= decayIncrement;
            if (output <= sustain)
            {
                state = Sustain;
            }
            break;
        case Sustain:
            break;
        case Release:
            output -= releaseIncrement;
            if (output <= .001)
            {
                state = Idle;
            }
            break;
    }
    
    return output;
}

void Envelope::setAttack(double newValue)
{
    attack = newValue;
    update();
}

void Envelope::setDecay(double newValue)
{
    decay = newValue;
    update();
}

void Envelope::setSustain(double newValue)
{
    sustain = newValue;
}

void Envelope::setRelease(double newValue)
{
    release = newValue;
    update();
}

Envelope::State Envelope::getState() const
{
    return state;
}

void Envelope::update()
{
    attackIncrement = 1 / (attack / 1000 * sampleRate);
    decayIncrement = 1 / (decay / 1000 * sampleRate);
    releaseIncrement = 1 / (release / 1000 * sampleRate);
}
