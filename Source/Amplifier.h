/*
  ==============================================================================

    Amplifier.h
    Created: 6 Jul 2019 6:31:20pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

class Amplifier
{
public:
    Amplifier();
    ~Amplifier();
    
    double processSample(double sample, int channel, double envMod);
    
    // parameter setters
    void setGain(double newValue);
    void setPan(double newValue);
    
private:
    
    // parameters
    double gain;
    double pan;
};

using Amp = Amplifier;
