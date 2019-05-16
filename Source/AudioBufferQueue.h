/*
  ==============================================================================

    AudioBufferQueue.h
    Created: 15 May 2019 7:20:34pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioBufferQueue
{
public:
    static constexpr size_t order = 9;
    static constexpr size_t bufferSize = 1U << order;
    static constexpr size_t numBuffers = 5;
    
    AudioBufferQueue();
    ~AudioBufferQueue();
    
    void push(const float* dataToPush, size_t numSamples);
    void pop (float* outputBuffer);
    
private:
    AbstractFifo abstractFifo { numBuffers };
    std::array<std::array<float, bufferSize>, numBuffers> buffers;
    };
