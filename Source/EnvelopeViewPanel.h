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
    
    void envelopeChanged(float attackRate, float decayRate, float sustainLevel, float releaseRate);
    
private:

    Rectangle<int> segmentView;

    Point<float> originPoint;
    
    Point<float> attackPoint;
    double attackCoefficient;
    double attackOffset;
    double attackTCO;
    
    Point<float> decayPoint;
    double decayCoefficient;
    double decayOffset;
    double decayTCO;
    
    Point<float> releasePoint;
    double releaseCoefficient;
    double releaseOffset;
    double releaseTCO;
};
