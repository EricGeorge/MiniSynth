/*
  ==============================================================================

    NaiveOscillator.cpp
    Created: 13 May 2019 8:15:21pm
    Author:  Eric

  ==============================================================================
*/

#include "TrivialOscillator.h"

#include "OscillatorParameters.h"
#include "PluginHelpers.h"

TrivialOscillator::TrivialOscillator(double sampleRate)
:   sampleRate(sampleRate),
    phaseInc(0.0f),
    phase(0.0f),
    waveform(Waveform_Sine),
    octaves(0),
    semitones(0),
    cents(0),
    pulseWidth(50.0f),
    noteOn(false)
{
}

TrivialOscillator::~TrivialOscillator()
{
}

void TrivialOscillator::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
        layout.add(std::make_unique<AudioParameterInt>(oscillatorWaveform_ParameterID, "Waveform", 1, 4, 1));
        layout.add(std::make_unique<AudioParameterInt>(oscillatorOctave_ParameterID, "Octave", -4, 4, 0));
        layout.add(std::make_unique<AudioParameterInt>(oscillatorSemitone_ParameterID, "Semitones", -12, 12, 0));
        layout.add(std::make_unique<AudioParameterInt>(oscillatorCents_ParameterID, "Cents", -100, 100, 0));
        layout.add(std::make_unique<AudioParameterFloat>(oscillatorPulseWidth_ParameterID, "Pulse Width", NormalisableRange<float> (0.0f, 100.0f), 50.0f));
}

void TrivialOscillator::addParameterListeners(AudioProcessorValueTreeState& state)
{
    state.addParameterListener(oscillatorWaveform_ParameterID, this);
    state.addParameterListener(oscillatorOctave_ParameterID, this);
    state.addParameterListener(oscillatorSemitone_ParameterID, this);
    state.addParameterListener(oscillatorCents_ParameterID, this);
    state.addParameterListener(oscillatorPulseWidth_ParameterID, this);
}

void TrivialOscillator::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    state.removeParameterListener(oscillatorWaveform_ParameterID, this);
    state.removeParameterListener(oscillatorOctave_ParameterID, this);
    state.removeParameterListener(oscillatorSemitone_ParameterID, this);
    state.removeParameterListener(oscillatorCents_ParameterID, this);
    state.removeParameterListener(oscillatorPulseWidth_ParameterID, this);
}

void TrivialOscillator::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == oscillatorWaveform_ParameterID)
    {
        waveform = static_cast<Waveform>(newValue);
    }
    else if (parameterID == oscillatorOctave_ParameterID)
    {
        octaves = static_cast<int>(newValue);
    }
    else if (parameterID == oscillatorSemitone_ParameterID)
    {
        semitones = static_cast<int>(newValue);
    }
    else if (parameterID == oscillatorCents_ParameterID)
    {
        cents = static_cast<int>(newValue);
    }
    else if (parameterID == oscillatorPulseWidth_ParameterID)
    {
        pulseWidth = newValue;
    }
}

void TrivialOscillator::reset(double sampleRate)
{
    sampleRate = sampleRate;
    phaseInc = 0.0f;
    phase = 0.0f;
    noteOn = false;
}

void TrivialOscillator::startNote(double frequency)
{
    float pitchShift = (octaves * 12) + semitones + ((float)cents / 100.0f);
    float modFreq = pitchShiftMultiplier(pitchShift);
    phaseInc = frequency * modFreq / sampleRate;
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
        switch ((Waveform)waveform)
        {
            case Waveform_Sine:
                sample = (float)(std::sin(phase * 2.0 * MathConstants<double>::pi));
                break;
            case Waveform_Saw:
                sample = unipolarToBipolar(phase);
                break;
            case Waveform_Triangle:
                sample = 2.0f * fabs(2.0f * phase - 1.0f) - 1.0f;
                break;
            case Waveform_Pulse:
                sample = phase > pulseWidth / 100.0f ? -1.0f : 1.0f;
                break;
        }
        
        phase += phaseInc;
        if (phase > 1.0f)
            phase -= 1.0f;
    }
    
    return sample;
}
