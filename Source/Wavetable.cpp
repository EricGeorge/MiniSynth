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

void BandLimitedWaveform::create(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm, double inTopFreq)
{
    // convert to time domain
    int numSamples = static_cast<int>(freqWaveRe.size());
    assert(numSamples == freqWaveIm.size());
    
    fft(numSamples, freqWaveRe, freqWaveIm);
    
    double scale = 1.0 / numSamples;
    
    // normalize (note we're also converting to floats here for the final sample vector)
    samples = std::vector<float>(freqWaveRe.begin(), freqWaveRe.end());
    std::for_each(samples.begin(), samples.end(), [scale](float &sample){ sample = sample * scale; });
    
    // check for dc offset and adjust if necessary
    auto max = FloatVectorOperations::findMaximum(samples.data(), numSamples);
    auto min = FloatVectorOperations::findMinimum(samples.data(), numSamples);

    auto offset = (max + min) / 2;
    if (abs(offset) > 0.001)
    {
        std::for_each(samples.begin(), samples.end(), [offset](float &sample){ sample = sample - offset; });
    }
    
    setTopFrequency(inTopFreq);
}

void BandLimitedWaveform::create2(std::vector<float>& freqWaveRe, double inTopFreq)
{
    // convert to time domain
    dsp::FFT fft(11);
    
    fft.performRealOnlyInverseTransform(&freqWaveRe[0]);
    
    samples.resize(2048);
    
    for (int i = 0; i < samples.size(); i++)
    {
        samples[i] = freqWaveRe[i];
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
void WavetableFrame::create(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm)
{
    int numSamples = static_cast<int>(freqWaveRe.size());
    assert(numSamples == freqWaveIm.size());
    
    // zero DC offset and Nyquist
    freqWaveRe[0] = freqWaveIm[0] = 0.0;
    freqWaveRe[numSamples >> 1] = freqWaveIm[numSamples >> 1] = 0.0;
    
    // determine maxHarmonic, the highest non-zero harmonic in the wave
    unsigned int maxHarmonic  = calculateMaxHarmonic(freqWaveRe, freqWaveIm);
    
    // topFreq is normalized to the sampleRate.
    // we can allow for aliasing back down to about 15kHz since it will be mostly inaudible.
    // so we can allow harmonics up to about (44.1k - 15k =) 29.1k.  29.1k / 22050 = 1.319
    // for simplicity - let's call it 1.3333 or 4/3 * nonaliased max.  That means our normal
    // max calculation of 1 / (2 * maxHarmonic) becomes 2 / (3 * maxHarmonic) or 2 / 3 / maxHarmonic
    double topFreq = 2.0 / 3.0 / maxHarmonic;
    
    // for subsquent tables, double topFreq and remove upper half of harmonics
    std::vector<double> ar(numSamples, 0.0);
    std::vector<double> ai(numSamples, 0.0);
    
    while (maxHarmonic > 0)
    {
        std::fill(ar.begin(), ar.end(), 0);
        std::fill(ai.begin(), ai.end(), 0);
        
        // fill the table in with the needed harmonics
        for (int idx = 1; idx <= maxHarmonic; idx++)
        {
            ar[idx] = freqWaveRe[idx];
            ai[idx] = freqWaveIm[idx];
            ar[numSamples - idx] = freqWaveRe[numSamples - idx];
            ai[numSamples - idx] = freqWaveIm[numSamples - idx];
        }
        
        // make the wavetable
        BandLimitedWaveform blWaveform;
        blWaveform.create(ar, ai, topFreq);
        blWaveforms.push_back(blWaveform);
        
        // prepare for next table
        topFreq *= 2;
        maxHarmonic >>= 1;
    }
}

void WavetableFrame::create2(std::vector<float>& freqData)
{
    int numSamples = static_cast<int>(freqData.size());
    
    // zero DC offset
    freqData[0] = 0.0;
    freqData[1] = 0.0;
    
    // determine maxHarmonic, the highest non-zero harmonic in the wave
    int maxHarmonic = numSamples >> 2;  // TODO - make this a function of FFT size - we should be passing that around.

    // topFreq is normalized to the sampleRate.
    // we can allow for aliasing back down to about 15kHz since it will be mostly inaudible.
    // so we can allow harmonics up to about (44.1k - 15k =) 29.1k.  29.1k / 22050 = 1.319
    // for simplicity - let's call it 1.3333 or 4/3 * nonaliased max.  That means our normal
    // max calculation of 1 / (2 * maxHarmonic) becomes 2 / (3 * maxHarmonic) or 2 / 3 / maxHarmonic
    double topFreq = 2.0 / 3.0 / maxHarmonic;
    
    // for subsquent tables, double topFreq and remove upper half of harmonics
    std::vector<float> ar(numSamples, 0.0);
    
    while (maxHarmonic > 0)
    {
        std::fill(ar.begin(), ar.end(), 0);
        
        int index = 1;
        int idx = index;
        
        // fill the table in with the needed harmonics
        for (index = 1; index <= maxHarmonic; index++)
        {
            idx = 2 * index;
            ar[idx] = freqData[idx];
            ar[idx + 1] = freqData[idx + 1];
            ar[numSamples - idx] = freqData[numSamples - idx];
            ar[numSamples - idx + 1] = freqData[numSamples - idx + 1];
        }
                
        // make the wavetable
        BandLimitedWaveform blWaveform;
        blWaveform.create2(ar, topFreq);
        blWaveforms.push_back(blWaveform);
        
        // prepare for next table
        topFreq *= 2;
        maxHarmonic >>= 1;
    }
}

// instead of a fixed table per octave, this builds tables based on the frequency constraints passed in
// minTopFrequency is the minimum normalized frequency that all wave tables support - ie ensures harmonics are produced to this value
// maxTopFrequency is the maximum normalized frequency that all wave tables support - determines how much aliasing is included.
void WavetableFrame::create(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm, double minTopFrequency, double maxTopFrequency)
{
    int numSamples = static_cast<int>(freqWaveRe.size());
    assert(numSamples == freqWaveIm.size());
    assert(maxTopFrequency > minTopFrequency);

    // zero DC offset and Nyquist to be safe
    freqWaveRe[0] = freqWaveIm[0] = 0.0;
    freqWaveRe[numSamples >> 1] = freqWaveIm[numSamples >> 1] = 0.0;
    
    // for subsequent tables, double topFreq and remove upper half of harmonics
    std::vector<double> ar(numSamples, 0.0);
    std::vector<double> ai(numSamples, 0.0);

    unsigned int maxHarmonic  = calculateMaxHarmonic(freqWaveRe, freqWaveIm);
    
    while (maxHarmonic > 0)
    {
        double topFreq = maxTopFrequency / maxHarmonic;
        
        std::fill(ar.begin(), ar.end(), 0);
        std::fill(ai.begin(), ai.end(), 0);

        // fill the table in with the needed harmonics
        for (int idx = 1; idx <= maxHarmonic; idx++)
        {
            ar[idx] = freqWaveRe[idx];
            ai[idx] = freqWaveIm[idx];
            ar[numSamples - idx] = freqWaveRe[numSamples - idx];
            ai[numSamples - idx] = freqWaveIm[numSamples - idx];
        }
        
        // make the wavetable
        BandLimitedWaveform blWaveform;
        blWaveform.create(ar, ai, topFreq);
        blWaveforms.push_back(blWaveform);
        
        // topFreq is new base frequency, so figure how many harmonics will fit withing maxTop
        int candidateHarmonic = std::round(minTopFrequency / topFreq);  // next table's maximum harmonic
        maxHarmonic = candidateHarmonic >= maxHarmonic ? maxHarmonic - 1 : candidateHarmonic;
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

void Wavetable::addFrame(std::vector<double>& freqWaveRe, std::vector<double>& freqWaveIm)
{
    WavetableFrame frame;
    frame.create(freqWaveRe, freqWaveIm);
    frames.push_back(frame);
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

void Wavetable::WriteFrameToWaveFile(String fileName, int frameID)
{
    frames[frameID].writeToWaveFile(fileName);
}

