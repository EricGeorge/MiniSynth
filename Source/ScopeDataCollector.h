/*
  ==============================================================================

    ScopeDataCollector.h
    Created: 15 May 2019 7:21:39pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioBufferQueue.h"

class ScopeDataCollector
{
public:
    ScopeDataCollector(AudioBufferQueue& queueToUse);
    ~ScopeDataCollector();
    
    void process(const float* data, size_t numSamples);
    
private:
    AudioBufferQueue& audioBufferQueue;
    
    std::array<float, AudioBufferQueue::bufferSize> buffer;
    size_t numCollected;
    
    float prevSample = 100.0f;
    static constexpr auto triggerLevel = 0.0001f;
    
    enum class State { waitingForTrigger, collecting } state { State::waitingForTrigger };
};
