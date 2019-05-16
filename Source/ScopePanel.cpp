/*
  ==============================================================================

    ScopeComponent.cpp
    Created: 16 May 2019 9:39:44am
    Author:  Eric

  ==============================================================================
*/

#include "ScopePanel.h"

ScopePanel::ScopePanel(Queue& queueToUse)
:   audioBufferQueue (queueToUse)
{
    sampleData.fill(0.0f);
    setFramesPerSecond(30);
}

ScopePanel::~ScopePanel()
{
    
}

void ScopePanel::setFramesPerSecond(int framesPerSecond)
{
    jassert(framesPerSecond > 0 && framesPerSecond < 1000);
    startTimerHz(framesPerSecond);
}

void ScopePanel::paint(Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    
    auto area = getLocalBounds();
    auto h = (float)area.getHeight();
    auto w = (float)area.getWidth();
    
    // Oscilloscope
    auto scopeRect = Rectangle<float> { 0.0f, 0.0f, w, h };
    plot (sampleData.data(), sampleData.size(), g, scopeRect, 1.0f, h/2);
}

void ScopePanel::resized()
{
    
}

void ScopePanel::timerCallback()
{
    audioBufferQueue.pop(sampleData.data());
    repaint();
}

void ScopePanel::plot(const float* data,
                          size_t numSamples,
                          Graphics& g,
                          juce::Rectangle<float> rect,
                          float scaler,
                          float offset)
{
    auto w = rect.getWidth();
    auto h = rect.getHeight();
    auto right = rect.getRight();
    
    auto center = rect.getBottom() - offset;
    auto gain = h * scaler;
    
    for (size_t i = 1; i < numSamples; ++i)
    {
        g.drawLine({ jmap<float>(i - 1, 0.0f, numSamples - 1, right - w, right),
            center - gain * data[i - 1],
            jmap<float>(i, 0.0f, numSamples - 1, right - w, right),
            center - gain * data[i] });
    }
}
