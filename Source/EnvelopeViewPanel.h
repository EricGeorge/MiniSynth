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
    Point<float> originPoint;
    Point<float> attackPoint;
    Point<float> decayPoint;
    Point<float> releasePoint;
    
    float maxSegmentWidth;
    Rectangle<int> segmentView;
    
    double numAttackPixels;
    double attackCoefficient;
    double attackOffset;
    double attackTCO;
    
    double numDecayPixels;
    double decayCoefficient;
    double decayOffset;
    double decayTCO;
    
    double numReleasePixels;
    double releaseCoefficient;
    double releaseOffset;
    double releaseTCO;
    
    double sampleRate;  // TODO - figure out how to get this here
};
