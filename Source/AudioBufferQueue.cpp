/*
  ==============================================================================

    AudioBufferQueue.cpp
    Created: 16 May 2019 9:39:32am
    Author:  Eric

  ==============================================================================
*/

#include "AudioBufferQueue.h"

AudioBufferQueue::AudioBufferQueue()
{
    
}

AudioBufferQueue::~AudioBufferQueue()
{
    
}

void AudioBufferQueue::push(const float* dataToPush, size_t numSamples)
{
    jassert(numSamples <= bufferSize);
    
    int start1, size1, start2, size2;
    abstractFifo.prepareToWrite(1, start1, size1, start2, size2);
    
    jassert(size1 <= 1);
    jassert(size2 == 0);
    
    if(size1 > 0)
        FloatVectorOperations::copy(buffers[(size_t) start1].data(), dataToPush, (int) jmin(bufferSize, numSamples));
    
    abstractFifo.finishedWrite(size1);
}

void AudioBufferQueue::pop (float* outputBuffer)
{
    int start1, size1, start2, size2;
    abstractFifo.prepareToRead(1, start1, size1, start2, size2);
    
    jassert(size1 <= 1);
    jassert(size2 == 0);
    
    if(size1 > 0)
        FloatVectorOperations::copy(outputBuffer, buffers[(size_t) start1].data(), (int) bufferSize);
    
    abstractFifo.finishedRead(size1);
}
