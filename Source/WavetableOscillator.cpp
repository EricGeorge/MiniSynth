/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#include "WavetableOscillator.h"

#include "PluginHelpers.h"

WavetableOscillator::WavetableOscillator(double sampleRate)
:   sampleRate(sampleRate),
    frequency(0.0),
    position(0.0),
    interpolate(false),
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

WavetableOscillator::~WavetableOscillator()
{

}

void WavetableOscillator::setPosition(float newValue)
{
    position = newValue;
}

void WavetableOscillator::setInterpolate(float newValue)
{
    interpolate = static_cast<bool>(newValue);
}

void WavetableOscillator::setSemitones(float newValue)
{
    semitones = static_cast<int>(newValue);
    update();
}

void WavetableOscillator::setCents(float newValue)
{
    cents = newValue;
    update();
}

void WavetableOscillator::setVolume(float newValue)
{
    volume = newValue;
}

void WavetableOscillator::reset(double inSampleRate)
{
    sampleRate = inSampleRate;
    frequency = 0.0;
    noteOn = false;
    
    sawOsc();
}

void WavetableOscillator::update()
{
    double modFrequency = getModFrequency(frequency, semitones + cents);
    phaseAccumulator.reset(0.5, modFrequency / sampleRate); // TODO - currently hard coded for saw
    
    // update the current wave table selector
    wavetable.setWaveform(modFrequency);
}

void WavetableOscillator::start(double inFrequency)
{
    frequency = inFrequency;
    
    update();
    
    noteOn = true;
}

void WavetableOscillator::stop()
{
    noteOn = false;
}

float WavetableOscillator::getNextSample()
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
