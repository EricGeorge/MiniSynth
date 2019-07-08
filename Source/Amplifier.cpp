/*
  ==============================================================================

    Amp.cpp
    Created: 6 Jul 2019 6:31:20pm
    Author:  Eric

  ==============================================================================
*/

#include "Amplifier.h"

#include "AmplifierParameters.h"

Amplifier::Amplifier()
:   gain(ampGainInitialValue),
    pan(ampPanInitialValue)
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
