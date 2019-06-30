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
    
    void create(std::vector<float>& freqWaveRe, double inTopFreq);

    double getTopFrequency() const;
    void setTopFrequency(double topFrequency);
    
    int getNumSamples() const;
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
    
    void create(std::vector<float>& freqWaveRe);

    const BandLimitedWaveform& getWaveform(int waveformIndex) const;
    int getNumWaveforms() const;

    void writeToWaveFile(String fileName);

private:
    std::vector<BandLimitedWaveform> blWaveforms;
    
};

class Wavetable
{
public:
    Wavetable();
    ~Wavetable();
    
    void addFrame(WavetableFrame& frame);

    void clear();
    
    bool wavetableLoaded() const;
    
    const WavetableFrame& getFrame(int frameIndex) const;
    int getNumFrames() const;
    
    void WriteFrameToWaveFile(String fileName, int frameID);
    
private:
    std::vector<WavetableFrame> frames;
};

inline WavetableFrame createFrameFromSingleCycle(std::vector<float> waveSamples)
{
    dsp::FFT fft(log2(kSingleCycleWaveformSize));

    std::vector<float> fftData(kSingleCycleWaveformSize * 2, 0.0f);
    std::copy(waveSamples.begin(), waveSamples.end(), fftData.begin());
    
    fft.performRealOnlyForwardTransform(fftData.data());
    
    WavetableFrame frame;
    frame.create(fftData);
    return frame;
}

