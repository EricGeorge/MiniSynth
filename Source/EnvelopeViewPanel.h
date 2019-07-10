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
    float attackX;
    float decayX;
    float sustainY;
    float releaseX;
    
    float maxSegmentWidth;
    float maxSegmentHeight;
};
