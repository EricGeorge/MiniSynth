/*
  ==============================================================================

    ScopeDataCollector.cpp
    Created: 16 May 2019 9:39:52am
    Author:  Eric

  ==============================================================================
*/

#include "ScopeDataCollector.h"

ScopeDataCollector::ScopeDataCollector(AudioBufferQueue& queueToUse)
:   audioBufferQueue(queueToUse),
    numCollected(0)
{
    
}

ScopeDataCollector::~ScopeDataCollector()
{
    
}

void ScopeDataCollector::process(const float* data, size_t numSamples)
{
    size_t index = 0;
    
    if (state == State::waitingForTrigger)
    {
        while (index++ < numSamples)
        {
            auto currentSample = *data++;
            
            if (currentSample >= triggerLevel && prevSample < triggerLevel)
            {
                numCollected = 0;
                state = State::collecting;
                break;
            }
            
            prevSample = currentSample;
        }
    }
    
    if (state == State::collecting)
    {
        while (index++ < numSamples)
        {
            buffer[numCollected++] = *data++;
            
            if (numCollected == buffer.size())
            {
                audioBufferQueue.push(buffer.data(), buffer.size());
                state = State::waitingForTrigger;
                prevSample = 100.0f;
                break;
            }
        }
    }
}
