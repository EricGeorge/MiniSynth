/*
  ==============================================================================

    Envelope.cpp
    Created: 6 Jul 2019 5:30:04pm
    Author:  Eric

  ==============================================================================
*/

#include "Envelope.h"

Envelope::Envelope(double sampleRate)
:   sampleRate(sampleRate),
    state(Idle),
    attack(0.1),
    decay(0.1),
    sustain(0.7),
    release(0.1)
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


