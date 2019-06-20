/*
  ==============================================================================

    Wavetable.h
    Created: 6 Jun 2019 5:42:14pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <vector>

#include "OscillatorHelpers.h"
#include "PluginHelpers.h"

class BandLimitedWaveform
{
public:
    BandLimitedWaveform();
    ~BandLimitedWaveform();
    
    void create(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm, double inTopFreq);
    
    double getTopFrequency() const;
    void setTopFrequency(double topFrequency);
    
    size_t getNumSamples() const;
    float getSample(int index) const;
    
private:
    double topFrequency;    // normalized to sampleRate
    std::vector<float> samples;
};

class WavetableFrame
{
public:
    WavetableFrame();
    ~WavetableFrame();
    
    void create(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm);
    void create(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm, double minTopFrequency, double maxTopFrequency);

    const BandLimitedWaveform& getWaveform(int waveformIndex) const;
    size_t getNumWaveforms() const;

    void writeToWaveFile(String fileName);

private:
    std::vector<BandLimitedWaveform> blWaveforms;
    
};

class Wavetable
{
public:
    Wavetable();
    ~Wavetable();

    void addFrame(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm);
    void addFrame(WavetableFrame& frame);

    void clear();
    
    const WavetableFrame& getFrame(int frameIndex) const;
    size_t getNumFrames() const;
    
    void WriteFrameToWaveFile(String fileName, int frameID);
    
private:
    std::vector<WavetableFrame> frames;
};

//
// example that builds a sawtooth oscillator via frequency domain
//
inline WavetableFrame createFrameFromSawWave()
{
    int tableLen = kSingleCycleWaveformSize;    // to give full bandwidth from 20 Hz
    int idx;
    std::vector<double> freqWaveRe;
    std::vector<double> freqWaveIm;
    
    freqWaveRe.resize(tableLen);
    freqWaveIm.resize(tableLen);
    
    // make a sawtooth
    for (idx = 0; idx < tableLen; idx++)
    {
        freqWaveIm[idx] = 0.0;
    }
    
    freqWaveRe[0] = freqWaveRe[tableLen >> 1] = 0.0;
    for (idx = 1; idx < (tableLen >> 1); idx++)
    {
        freqWaveRe[idx] = 1.0 / idx;                    // sawtooth spectrum
        freqWaveRe[tableLen - idx] = -freqWaveRe[idx];  // mirror
    }
    
    WavetableFrame frame;
    frame.create(freqWaveRe, freqWaveIm);
    return frame;
}

//
// example that creates and oscillator from an arbitrary time domain wave
//
inline WavetableFrame createFrameFromSingleCycle(std::vector<float> waveSamples)
{
    int tableLength = static_cast<int>(waveSamples.size());
    
    int idx;
    std::vector<double> freqWaveRe;
    std::vector<double> freqWaveIm;
    freqWaveRe.resize(tableLength);
    freqWaveIm.resize(tableLength);
    
    // take FFT
    for (idx = 0; idx < tableLength; idx++)
    {
        freqWaveIm[idx] = waveSamples[idx];
        freqWaveRe[idx] = 0.0;
    }
    
    // Note - this fft (to freq domain) is the inverse of the other FFs later in the
    // calculation which are returning to time domain.
    fft(tableLength, freqWaveIm, freqWaveRe);
    
    WavetableFrame frame;
    frame.create(freqWaveRe, freqWaveIm);
    return frame;
}
