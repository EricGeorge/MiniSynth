/*
  ==============================================================================

    EnvelopeViewPanel.cpp
    Created: 9 Jul 2019 3:56:29pm
    Author:  Eric

  ==============================================================================
*/

#include "EnvelopeViewPanel.h"

#include "EnvelopeParameters.h"
#include "PluginCommonStyling.h"
#include "PluginHelpers.h"

EnvelopeViewPanel::EnvelopeViewPanel()
:   originPoint(0,0),
    attackPoint(0, 0),
    decayPoint(0, 0),
    releasePoint(0, 0),
    maxSegmentWidth(0),
    segmentView(0, 0, 0, 0),
    numAttackPixels(0.0),
    attackCoefficient(0.0),
    attackOffset(0.0),
    numDecayPixels(0.0),
    decayCoefficient(0.0),
    decayOffset(0.0),
    numReleasePixels(0.0),
    releaseCoefficient(0.0),
    releaseOffset(0.0),
    sampleRate(44100)
{
    attackTCO = exp(-1.5);
    decayTCO = exp(-4.95);
    releaseTCO = decayTCO;
}

EnvelopeViewPanel::~EnvelopeViewPanel()
{
    
}

void EnvelopeViewPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    Rectangle<int> border(getLocalBounds());

    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    // paint the envelope
    g.setColour(getCommonColours().detailContrast);

    Path envelopePath;
    envelopePath.startNewSubPath(originPoint);

    float envelope = 0;
    for (int sample = 0; sample < numAttackPixels; sample++)
    {
        envelope = attackOffset + envelope * attackCoefficient;
        envelopePath.lineTo(sample, segmentView.getBottom() - envelope);
    }
    
    for (int sample = numAttackPixels; sample < numAttackPixels + numDecayPixels; sample++)
    {
        envelope = decayOffset + envelope * decayCoefficient;
        envelopePath.lineTo(sample, segmentView.getBottom() - envelope);
    }
    
    for (int sample = numAttackPixels + numDecayPixels; sample < numAttackPixels + numDecayPixels + numReleasePixels; sample++)
    {
        envelope = releaseOffset + envelope * releaseCoefficient;
        envelopePath.lineTo(sample, segmentView.getBottom() - envelope);
    }
    
//    envelopePath.startNewSubPath(originPoint);
//    envelopePath.lineTo(attackPoint);
//    envelopePath.lineTo(decayPoint);
//    envelopePath.lineTo(releasePoint);
    
    PathStrokeType strokeType(3.0f);
    g.strokePath(envelopePath, strokeType);

    ColourGradient envelopeGradient(getCommonColours().detailContrast,
                                    segmentView.getX() + segmentView.getWidth() / 2,
                                    segmentView.getY(),
                                    getCommonColours().panelBackground,
                                    segmentView.getX() + segmentView.getWidth() / 2,
                                    segmentView.getBottom(),
                                    false);

    g.setGradientFill(envelopeGradient);
    g.fillPath(envelopePath);
    
    // draw the control dots
    g.setColour(Colours::ghostwhite);
    int radius = 8;
    int diameter = 2 * radius;
    g.fillEllipse(attackPoint.getX() - radius, segmentView.getY() - radius, diameter, diameter);
    g.fillEllipse(decayPoint.getX() - radius, decayPoint.getY() - radius, diameter, diameter);
    g.fillEllipse(releasePoint.getX() - radius, segmentView.getBottom() - radius, diameter, diameter);

    // paint the border
    g.setColour(getCommonColours().detail);
    g.drawRect(border, 3.0);
}

void EnvelopeViewPanel::resized()
{
    segmentView = getLocalBounds();
    segmentView.removeFromLeft(5);
    segmentView.removeFromTop(20);
    segmentView.removeFromRight(20);
    segmentView.removeFromBottom(4);
    
    maxSegmentWidth = segmentView.getWidth() / 3;
}

void EnvelopeViewPanel::envelopeChanged(float attackRate, float decayRate, float sustainLevel, float releaseRate)
{
    originPoint.setX(segmentView.getBottomLeft().getX());
    originPoint.setY(segmentView.getBottomLeft().getY());

    attackPoint.setX(segmentView.getX() + convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attackRate, 0.5, 1000.0) * maxSegmentWidth);
    attackPoint.setY(segmentView.getY());

    decayPoint.setX(attackPoint.getX() + convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decayRate, 0.5, 1000.0) * maxSegmentWidth);
    decayPoint.setY(segmentView.getBottom() - sustainLevel * segmentView.getHeight());

    releasePoint.setX(decayPoint.getX() + convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, releaseRate, 0.5, 1000.0) * maxSegmentWidth);
    releasePoint.setY(segmentView.getBottom());

    numAttackPixels = convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attackRate, 0.5, 1000.0) * maxSegmentWidth;
    attackCoefficient = std::exp(-std::log((1.0 + attackTCO) / attackTCO) / numAttackPixels);
    attackOffset = (segmentView.getHeight() + attackTCO) * (1.0 - attackCoefficient);

    numDecayPixels = convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decayRate, 0.5, 1000.0) * maxSegmentWidth;
    decayCoefficient = std::exp(-std::log((1.0 + decayTCO) / decayTCO) / numDecayPixels);
    decayOffset = (segmentView.getHeight() * sustainLevel  - decayTCO) * (1.0 - decayCoefficient);
    
    numReleasePixels = convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, releaseRate, 0.5, 1000.0) * maxSegmentWidth;
    releaseCoefficient = std::exp(-std::log((1.0 + releaseTCO) / releaseTCO) / numReleasePixels);
    releaseOffset = -releaseTCO * (1.0 - releaseCoefficient);
    
    repaint();
}
