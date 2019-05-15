/*
  ==============================================================================

    NaiveOscillator.cpp
    Created: 13 May 2019 8:15:21pm
    Author:  Eric

  ==============================================================================
*/

#include "TrivialOscillator.h"
#include "../JuceLibraryCode/JuceHeader.h"

TrivialOscillator::TrivialOscillator(double sampleRate)
:   sampleRate(sampleRate),
    phaseInc(0.0f),
    phase(0.0f),
    waveform(Waveform_Sine),
    noteOn(false)
{
}

TrivialOscillator::~TrivialOscillator()
{
}

void TrivialOscillator::reset(double sampleRate)
{
    sampleRate = sampleRate;
    phase = 0.0f;
    phaseInc = 0.0f;
    noteOn = false;
}

void TrivialOscillator::startNote(double frequency)
{
    phaseInc = frequency / sampleRate;
    noteOn = true;
}

void TrivialOscillator::stopNote()
{
    noteOn = false;
}

float TrivialOscillator::getNextSample()
{
    float sample = 0.0f;
    
    if (noteOn)
    {
        switch (waveform)
        {
            case Waveform_Sine:
                sample = (float)(std::sin(phase * 2.0 * MathConstants<double>::pi));
                break;
            case Waveform_Saw:
                sample = 2.0f * phase - 1.0f;
                break;
            case Waveform_Triangle:
                sample = 2.0f * fabs(2.0f * phase - 1.0f) - 1.0f;
                break;
            case Waveform_Square:
                sample = phase > 0.5f ? -1.0f : 1.0f;
                break;
        }
        
        phase += phaseInc;
        if (phase > 1.0f)
            phase -= 1.0f;
    }
    
    return sample;
}
