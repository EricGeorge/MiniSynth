/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#include "WavetableOscillator.h"

#include "PluginHelpers.h"

WavetableOscillator::WavetableOscillator(double sampleRate, Wavetable& wavetable)
:   sampleRate(sampleRate),
    frequency(0.0),
    position(0.0),
    interpolate(false),
    semitones(0),
    cents(0),
    volume(1.0),
    currentWaveformIndex(0),
    currentFrameIndex(0),
    wavetable(wavetable)
{
    WavetableFrame frame = sawOsc();
    wavetable.addFrame(frame);
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
    double normalizedModFrequency = modFrequency/sampleRate;

    phaseAccumulator.reset(0.0, normalizedModFrequency);
    
    // update the current wave table selector
    currentWaveformIndex = 0;
    while ((normalizedModFrequency >= currentFrame().getWaveform(currentWaveformIndex).getTopFrequency()) &&
           (currentWaveformIndex < (currentFrame().getNumWaveforms() - 1)))
    {
        ++currentWaveformIndex;
    }

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
        float index = phaseAccumulator.getPhase() * currentWaveform().getNumSamples();
        float frac = index - static_cast<int>(index);
        sample = linear_interp(currentWaveform().getSample(static_cast<int>(index)),
                               currentWaveform().getSample(static_cast<int>(index) + 1),
                               frac);

        phaseAccumulator.IncrementPhase();
    }
    
    return sample * volume;
}

const WavetableFrame& WavetableOscillator::currentFrame() const
{
    return wavetable.getFrame(currentFrameIndex);
}

const BandLimitedWaveform& WavetableOscillator::currentWaveform() const
{
    return currentFrame().getWaveform(currentWaveformIndex);
}
