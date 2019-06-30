/*
  ==============================================================================

    Wavetable.cpp
    Created: 6 Jun 2019 5:42:14pm
    Author:  Eric

  ==============================================================================
*/
#include "Wavetable.h"

#include "OscillatorHelpers.h"
#include "PluginHelpers.h"

// MARK: - BandLimitedWaveform
BandLimitedWaveform::BandLimitedWaveform()
:   topFrequency(0)
{
}

BandLimitedWaveform::~BandLimitedWaveform()
{
    
}

double BandLimitedWaveform::getTopFrequency() const
{
    return topFrequency;
}

void BandLimitedWaveform::setTopFrequency(double inTopFrequency)
{
    topFrequency = inTopFrequency;
}

int BandLimitedWaveform::getNumSamples() const
{
    return static_cast<int>(samples.size());
}

float BandLimitedWaveform::getSample(int index) const
{
    return samples[index];
}

void BandLimitedWaveform::create(std::vector<float>& freqData, double inTopFreq)
{
    // convert to time domain
    dsp::FFT fft(log2(kSingleCycleWaveformSize));
    fft.performRealOnlyInverseTransform(freqData.data());
    
    samples.assign(freqData.begin(), freqData.begin() + kSingleCycleWaveformSize);
    
    // check if waveform is centered around 0 and adjust if necessary
    auto minMax = std::minmax_element(samples.begin(), samples.end());
    
    float max = *minMax.second;
    float min = *minMax.first;
    
    float offset = (max + min) / 2;
    
    if (abs(offset) > 0.001)
    {
        std::for_each(samples.begin(), samples.end(), [offset](float &sample){ sample = sample - offset; });
    }
    
    setTopFrequency(inTopFreq);
}

// MARK: - WavetableFrame
WavetableFrame::WavetableFrame()
{
    blWaveforms.clear();
}

WavetableFrame::~WavetableFrame()
{
    
}

const BandLimitedWaveform& WavetableFrame::getWaveform(int waveformIndex) const
{
    return blWaveforms[waveformIndex];
}

int WavetableFrame::getNumWaveforms() const
{
    return static_cast<int>(blWaveforms.size());
}

// This generates a fixed 1 table per octave set of waveforms
void WavetableFrame::create(std::vector<float>& freqData)
{
    unsigned long numSamples = freqData.size();
    assert(numSamples == kSingleCycleWaveformSize * 2);
    
    // zero DC offset
    freqData[0] = 0.0;
    freqData[1] = 0.0;
    
    // determine maxHarmonic, the highest non-zero harmonic in the wave
    unsigned int maxHarmonic = calculateMaxHarmonic(freqData);

    // topFreq is normalized to the sampleRate.
    // we can allow for aliasing back down to about 15kHz since it will be mostly inaudible.
    // so we can allow harmonics up to about (44.1k - 15k =) 29.1k.  29.1k / 22050 = 1.319
    // for simplicity - let's call it 1.3333 or 4/3 * nonaliased max.  That means our normal
    // max calculation of 1 / (2 * maxHarmonic) becomes 2 / (3 * maxHarmonic) or 2 / 3 / maxHarmonic
    double topFreq = 2.0 / 3.0 / maxHarmonic;
    
    // for subsquent tables, double topFreq and remove upper half of harmonics
    std::vector<float> blFreqData(numSamples, 0.0);
    
    while (maxHarmonic > 0)
    {
        std::fill(blFreqData.begin(), blFreqData.end(), 0);
        
        int index = 1;
        
        // fill the table in with the needed harmonics
        for (index = 1; index <= maxHarmonic; index++)
        {
            int realIndex = 2 * index;
            int imagIndex = realIndex + 1;
            
            blFreqData[realIndex] = freqData[realIndex];
            blFreqData[imagIndex] = freqData[imagIndex];
            blFreqData[numSamples - realIndex] = freqData[numSamples - realIndex];
            blFreqData[numSamples - imagIndex] = freqData[numSamples - imagIndex];
        }
                
        // make the wavetable
        BandLimitedWaveform blWaveform;
        blWaveform.create(blFreqData, topFreq);
        blWaveforms.push_back(blWaveform);
        
        // prepare for next table
        topFreq *= 2;
        maxHarmonic >>= 1;
    }
}

void WavetableFrame::writeToWaveFile(String fileName)
{
    // create a buffer from the frame
    AudioSampleBuffer buffer;
    buffer.setSize(1, getNumWaveforms() * kSingleCycleWaveformSize);
    int bufferIndex = 0;
    
    for (int index = 0; index < blWaveforms.size(); index++)
    {
        for (int sample = 0; sample < blWaveforms[index].getNumSamples(); sample++)
        {
            buffer.setSample(0, bufferIndex++, blWaveforms[index].getSample(sample));
        }
    }
    
    WavAudioFormat format;
    String targetFolder = (File::getSpecialLocation(File::userHomeDirectory)).getFullPathName() + "/Downloads/WaveTableFrames/";
    
    if (!File(targetFolder).exists())
    {
        File(targetFolder).createDirectory();
    }

    File outputFile = File(targetFolder + fileName);
    if (outputFile.exists())
    {
        outputFile.deleteFile();
    }
    
    FileOutputStream* outputTo = outputFile.createOutputStream();
    std::unique_ptr<AudioFormatWriter> writer(format.createWriterFor(outputTo, 44100, 1, 16, NULL, 0));
    writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

// MARK: - Wavetable
Wavetable::Wavetable()
{
}

Wavetable::~Wavetable()
{
    
}

void Wavetable::addFrame(WavetableFrame& frame)
{
    frames.push_back(frame);
}

const WavetableFrame& Wavetable::getFrame(int frameIndex) const
{
    return frames[frameIndex];
}

int Wavetable::getNumFrames() const
{
    return static_cast<int>(frames.size());
}

void Wavetable::clear()
{
    frames.clear();
}

bool Wavetable::wavetableLoaded() const
{
    return frames.size() > 0;
}

void Wavetable::WriteFrameToWaveFile(String fileName, int frameID)
{
    frames[frameID].writeToWaveFile(fileName);
}

