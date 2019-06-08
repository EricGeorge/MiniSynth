/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#include "WavetableOscillator.h"

#include "PluginHelpers.h"

WaveTableOscillator::WaveTableOscillator(double sampleRate)
:   sampleRate(sampleRate),
    octaves(0),
    semitones(0),
    cents(0),
    volume(1.0),
    currentWaveform(0),
    wavetable(sampleRate)
{
    WavetableFrame frame = sawOsc();
    wavetable.addFrame(frame);
    wavetable.setCurrentFrame(0);
}

WaveTableOscillator::~WaveTableOscillator()
{

}

void WaveTableOscillator::setOctaves(float newValue)
{
    octaves = static_cast<int>(newValue);
}

void WaveTableOscillator::setSemitones(float newValue)
{
    semitones = static_cast<int>(newValue);
}

void WaveTableOscillator::setCents(float newValue)
{
    cents = static_cast<int>(newValue);
}

void WaveTableOscillator::setVolume(float newValue)
{
    volume = newValue;
}

void WaveTableOscillator::reset(double inSampleRate)
{
    sampleRate = inSampleRate;
    noteOn = false;
    
    sawOsc();
}

void WaveTableOscillator::start(double frequency)
{
    double modFrequency = frequency * getPitchFreqMod(octaves, semitones, cents);
    phaseAccumulator.reset(0.5, modFrequency / sampleRate); // TODO - currently hard coded for saw
    noteOn = true;
    
    // update the current wave table selector
    wavetable.setWaveform(frequency);
}

void WaveTableOscillator::stop()
{
    noteOn = false;
}

float WaveTableOscillator::getNextSample()
{
    float sample = 0.0f;
    
    if (noteOn)
    {
        float index = phaseAccumulator.getPhase() * wavetable.currentWaveform().getNumSamples();
        float frac = index - static_cast<int>(index);
        sample = linear_interp(wavetable.currentWaveform().getSample(static_cast<int>(index)),
                               wavetable.currentWaveform().getSample(static_cast<int>(index) + 1),
                               frac);

        phaseAccumulator.IncrementPhase();
    }
    
    return sample * volume;
}
