/*
  ==============================================================================

    Amp.cpp
    Created: 6 Jul 2019 6:31:20pm
    Author:  Eric

  ==============================================================================
*/

#include "Amplifier.h"

Amplifier::Amplifier()
:   gain(1.0),
    pan(0.0)
{
    
}

Amplifier::~Amplifier()
{
    
}

void Amplifier::setGain(double newValue)
{
    gain = newValue;
}

void Amplifier::setPan(double newValue)
{
    pan = newValue;
}

double Amplifier::processSample(double sample, int channel, double envMod)
{
    return sample * gain * envMod;
}
