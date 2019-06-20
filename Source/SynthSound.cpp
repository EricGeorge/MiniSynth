/*
  ==============================================================================

    SynthSound.cpp
    Created: 19 Jun 2019 7:08:39pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthSound.h"

SynthSound::SynthSound()
{
    
}

SynthSound::~SynthSound()
{
    
}

bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool SynthSound::appliesToChannel(int midiChannel)
{
    return true;
}

void SynthSound::setWavetableFile(const String wavFile)
{
    
}

Wavetable& SynthSound::getWavetable()
{
    return wavetable;
}
