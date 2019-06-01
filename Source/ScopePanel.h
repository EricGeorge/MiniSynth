/*
  ==============================================================================

    ScopeComponent.h
    Created: 15 May 2019 7:18:53pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioBufferQueue.h"

class ScopePanel : public Component,
private Timer
{
public:
    using Queue = AudioBufferQueue;
    
    ScopePanel(Queue& queueToUse);
    ~ScopePanel() override;
    
    void setFramesPerSecond(int framesPerSecond);
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    Queue& audioBufferQueue;
    std::array<float, Queue::bufferSize> sampleData {};

    void timerCallback() override;
    
    static void plot(const float* data,
                     size_t numSamples,
                     Graphics& g,
                     juce::Rectangle<float> rect,
                     float scaler = 1.0f,
                     float offset = 0.0f);
};
