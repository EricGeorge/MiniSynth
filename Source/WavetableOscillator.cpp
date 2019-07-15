/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#include "WavetableOscillator.h"

#include "PluginHelpers.h"
#include "WavetableParameters.h"

WavetableOscillator::WavetableOscillator(double sampleRate, SynthSound& sound)
:   sampleRate(sampleRate),
    frequency(0.0),
    sound(sound),
    position(wtbPositionInitialValue),
    interpolate(wtbInterpolateInitialValue),
    semitones(wtbSemitonesInitialValue),
    cents(wtbCentsInitialValue),
    volume(wtbVolumeInitialValue),
    currentWaveformIndex(0),
    currentFrameIndex(-1),
    nextFrameIndex(-1),
    trueFrameIndex(0.0),
    wavetable(sound.getWavetable())
{
    sound.addActionListener(this);
}

WavetableOscillator::~WavetableOscillator()
{
    sound.removeActionListener(this);
}

void WavetableOscillator::actionListenerCallback (const String& message)
{
    if (message == synthSound_BroadcastIDs[kSSBC_WavetableChanged])
    {
        currentFrameIndex = nextFrameIndex = -1;
        updateFrameIndices();
    }
}

void WavetableOscillator::updateFrameIndices()
{
    trueFrameIndex = position * wavetable.getNumFrames();
    
    // just see if the incremental index changed
    int newFrameIndex = trueFrameIndex;
    
    if (currentFrameIndex != newFrameIndex)
    {
        currentFrameIndex = newFrameIndex;
        
        nextFrameIndex = currentFrameIndex +1;
        if (nextFrameIndex >= wavetable.getNumFrames())
        {
            nextFrameIndex = currentFrameIndex;
        }
    }
}

void WavetableOscillator::setPosition(float newValue)
{
    position = newValue;
    updateFrameIndices();
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
}

void WavetableOscillator::update()
{
    double modFrequency = getModFrequency(frequency, semitones + cents);
    double normalizedModFrequency = modFrequency/sampleRate;

    phaseAccumulator.reset(normalizedModFrequency);
    
    // update the current wave table selector
    currentWaveformIndex = 0;
    while ((normalizedModFrequency >= currentFrame().getWaveform(currentWaveformIndex).getTopFrequency()) &&
           (currentWaveformIndex < (currentFrame().getNumWaveforms() - 1)))
    {
        ++currentWaveformIndex;
    }
}

void WavetableOscillator::setFrequency(double inFrequency)
{
    frequency = inFrequency;
    update();
}

float WavetableOscillator::getNextSample()
{
    float index = phaseAccumulator.getPhase() * currentWaveform().getNumSamples();
    float frac = index - static_cast<int>(index);
    float sample = linear_interp(currentWaveform().getSample(static_cast<int>(index)),
                           currentWaveform().getSample(static_cast<int>(index) + 1),
                           frac);

    if (interpolate && (nextFrameIndex != currentFrameIndex))
    {
        // interpolate between frames
        float frameFrac = trueFrameIndex - currentFrameIndex;
        
        float sampleNextFrame = linear_interp(nextFrameCurrentWaveform().getSample(static_cast<int>(index)),
                                              nextFrameCurrentWaveform().getSample(static_cast<int>(index) + 1),
                                              frac);
        
        sample = linear_interp(sample, sampleNextFrame, frameFrac);
    }
    
    phaseAccumulator.IncrementPhase();
    
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

const WavetableFrame&  WavetableOscillator::nextFrame() const
{
    return wavetable.getFrame(nextFrameIndex);
}

const BandLimitedWaveform& WavetableOscillator::nextFrameCurrentWaveform() const
{
    return nextFrame().getWaveform(currentWaveformIndex);
}

