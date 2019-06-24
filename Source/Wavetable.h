/*
  ==============================================================================

    Wavetable.h
    Created: 6 Jun 2019 5:42:14pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <vector>

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
    
    void setWaveform(double normalizedFrequency);
    
private:
    std::vector<BandLimitedWaveform> blWaveforms;
};

class Wavetable
{
public:
    Wavetable(double sampleRate);
    ~Wavetable();

    void addFrame(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm);
    void addFrame(WavetableFrame& frame);

    const WavetableFrame& getFrame(int frameIndex) const;
    const WavetableFrame& currentFrame() const;
    size_t getNumFrames() const;
    void setCurrentFrame(int currentFrame);
    
    const BandLimitedWaveform& currentWaveform() const;
    void setWaveform(double frequency);
    
private:
    std::vector<WavetableFrame> frames;
    double sampleRate;
    
    // state information
    int currentFrameIndex;
    int currentWaveformIndex;
};

//
// example that builds a sawtooth oscillator via frequency domain
//
inline WavetableFrame sawOsc()
{
    int tableLen = 2048;    // to give full bandwidth from 20 Hz
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
inline WavetableFrame waveOsc(double *waveSamples, int tableLen)
{
    int idx;
    std::vector<double> freqWaveRe;
    std::vector<double> freqWaveIm;
    freqWaveRe.resize(tableLen);
    freqWaveIm.resize(tableLen);
    
    // take FFT
    for (idx = 0; idx < tableLen; idx++)
    {
        freqWaveIm[idx] = waveSamples[idx];
        freqWaveRe[idx] = 0.0;
    }
    
    fft(tableLen, freqWaveRe, freqWaveIm);
    
    WavetableFrame frame;
    frame.create(freqWaveRe, freqWaveIm);
    return frame;
}
