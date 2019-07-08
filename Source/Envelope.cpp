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
    attack(envAttackInitialValue),
    decay(envDecayInitialValue),
    sustain(envSustainInitialValue),
    release(envReleaseInitialValue)
{
    
}

Envelope::~Envelope()
{
    
}

void Envelope::start()
{
    
}

void Envelope::end()
{
    
}

double Envelope::getNextSampleMod()
{
    switch (state)
    {
        case Idle:
            break;
        case Attack:
            break;
        case Decay:
            break;
        case Sustain:
            break;
        case Release:
            break;
    }
    
    return 1.0;
}

void Envelope::setAttack(double newValue)
{
    attack = newValue;
}

void Envelope::setDecay(double newValue)
{
    decay = newValue;
}

void Envelope::setSustain(double newValue)
{
    sustain = newValue;
}

void Envelope::setRelease(double newValue)
{
    release = newValue;
}


