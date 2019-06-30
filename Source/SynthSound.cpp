/*
  ==============================================================================

    SynthSound.cpp
    Created: 19 Jun 2019 7:08:39pm
    Author:  Eric

  ==============================================================================
*/

#include "SynthSound.h"

SynthSound::SynthSound()
{
    formatManager.registerBasicFormats();
}

SynthSound::~SynthSound()
{
    
}

bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool SynthSound::appliesToChannel(int midiChannel)
{
    return true;
}

void SynthSound::setWavetableFile(const String wavFile)
{
    AudioSampleBuffer fileBuffer;
    auto file = File(wavFile);
    
    wavetable.clear();

    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader.get() != nullptr)
    {
        fileBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&fileBuffer,
                     0,
                     (int) reader->lengthInSamples,
                     0,
                     true,
                     true);
    }
    
    auto size = fileBuffer.getNumSamples();
    auto channels = fileBuffer.getNumChannels();
    DBG(wavFile << ": " << size << " samples and "<< channels << " channels");

    // how  many frames will we have? (int throws away the leftover)
    int numFrames = size / kSingleCycleWaveformSize;
    
    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++)
    {
        std::vector<float> waveformSamples;
        int bufferOffset = frameIndex * kSingleCycleWaveformSize;
        
        for (int index = bufferOffset; index < kSingleCycleWaveformSize + bufferOffset; index++)
        {
            waveformSamples.push_back(fileBuffer.getSample(0, index));
        }
        
        wavetable.createFrame(waveformSamples);
    }
    
    sendActionMessage(synthSound_BroadcastIDs[kSSBC_WavetableChanged]);
}

Wavetable& SynthSound::getWavetable()
{
    return wavetable;
}
