/*
  ==============================================================================

    WavetableOscillator.h
    Created: 24 May 2019 9:34:37am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "OscillatorHelpers.h"
#include "SynthSound.h"
#include "Wavetable.h"

class WavetableOscillator : public ActionListener
{
public:
    WavetableOscillator(double sampleRate, SynthSound& sound);
    ~WavetableOscillator();
    
    float getNextSample();
    
    void reset(double sampleRate);
    void update();
    
    void start(double frequency);
    void stop();


    // parameter setters
    void setPosition(float newValue);
    void setInterpolate(float newValue);
    void setSemitones(float newValue);
    void setCents(float newValue);
    void setVolume(float newValue);

    void actionListenerCallback (const String& message) override;

private:
    double sampleRate;
    double frequency;
    SynthSound& sound;
    
    PhaseAccumulator phaseAccumulator;

    // parameters
    double position;
    bool interpolate;
    int semitones;
    double cents;
    double volume;
    
    // poor man's envelope - replace with ADSR
    bool noteOn;

    int currentWaveformIndex;      // current table, based on current frequency
    int currentFrameIndex;
    int nextFrameIndex;
    double trueFrameIndex;
    Wavetable& wavetable;
    
    void updateFrameIndices();
    
    const WavetableFrame& currentFrame() const;
    const BandLimitedWaveform& currentWaveform() const;
    
    const WavetableFrame& nextFrame() const;
    const BandLimitedWaveform& nextFrameCurrentWaveform() const;
};

using WTOsc = WavetableOscillator;



