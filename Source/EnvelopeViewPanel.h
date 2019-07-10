/*
  ==============================================================================

    EnvelopeViewPanel.h
    Created: 9 Jul 2019 3:56:29pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopeViewPanel : public Component
{
public:
    EnvelopeViewPanel();
    ~EnvelopeViewPanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void envelopeChanged(float attack, float decay, float sustain, float release);
    
private:
    Point<float> originPoint;
    Point<float> attackPoint;
    Point<float> decayPoint;
    Point<float> releasePoint;
    
    float maxSegmentWidth;
    Rectangle<int> segmentView;
};
