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
:   segmentView(0, 0, 0, 0),
    originPoint(0, 0),
    attackPoint(0, 0),
    attackCoefficient(0.0),
    attackOffset(0.0),
    decayPoint(0, 0),
    decayCoefficient(0.0),
    decayOffset(0.0),
    sustainLevel(0),
    releasePoint(0, 0),
    releaseCoefficient(0.0),
    releaseOffset(0.0)
{
}

EnvelopeViewPanel::~EnvelopeViewPanel()
{
    
}

void EnvelopeViewPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    Rectangle<int> border(getLocalBounds());
    
    // background
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    // envelope
    g.setColour(getCommonColours().detailContrast);

    Path envelopePath;
    envelopePath.startNewSubPath(originPoint);

    float envelope = 0;
    
    for (int pixel = originPoint.getX(); pixel < releasePoint.getX(); pixel++)
    {
        double offset = 0.0;
        double coefficient = 0.0;
        bool checkSustain = false;
        
        if (pixel < attackPoint.getX())
        {
            offset = attackOffset;
            coefficient = attackCoefficient;
        }
        else if (pixel < decayPoint.getX())
        {
            offset = decayOffset;
            coefficient = decayCoefficient;
            checkSustain = true;
        }
        else if (pixel < releasePoint.getX())
        {
            offset = releaseOffset;
            coefficient = releaseCoefficient;
        }
        
        envelope = offset + envelope * coefficient;
        
        if (checkSustain && envelope < sustainLevel)
        {
            envelope = sustainLevel;
            checkSustain = false;
        }
        
        envelopePath.lineTo(pixel, segmentView.getBottom() - envelope * segmentView.getHeight());
    }
    
    PathStrokeType strokeType(3.0f);
    g.strokePath(envelopePath, strokeType);

    // fill envelope
    ColourGradient envelopeGradient(getCommonColours().detailContrast,
                                    segmentView.getX() + segmentView.getWidth() / 2,
                                    segmentView.getY(),
                                    getCommonColours().panelBackground,
                                    segmentView.getX() + segmentView.getWidth() / 2,
                                    segmentView.getBottom(),
                                    false);

    g.setGradientFill(envelopeGradient);
    g.fillPath(envelopePath);
    
    // control dots
    g.setColour(Colours::ghostwhite);
    int radius = 8;
    int diameter = 2 * radius;
    g.fillEllipse(attackPoint.getX() - radius, segmentView.getY() - radius, diameter, diameter);
    g.fillEllipse(decayPoint.getX() - radius, decayPoint.getY() - radius, diameter, diameter);
    g.fillEllipse(releasePoint.getX() - radius, segmentView.getBottom() - radius, diameter, diameter);

    // border
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
}

void EnvelopeViewPanel::envelopeChanged(float attackRate,
                                        float decayRate,
                                        float inSustainLevel,
                                        float releaseRate,
                                        float attackCurve,
                                        float decayCurve,
                                        float releaseCurve)
{
    float maxSegmentWidth = segmentView.getWidth() / 3;

    originPoint.setX(segmentView.getBottomLeft().getX());
    originPoint.setY(segmentView.getBottomLeft().getY());

    sustainLevel = inSustainLevel;
    
    // Attack
    float numAttackPixels = convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attackRate, 0.5, 1000.0) * maxSegmentWidth;
    attackPoint.setX(segmentView.getX() + numAttackPixels);
    attackPoint.setY(segmentView.getY());
    
    attackCoefficient = std::exp(-std::log((1.0 + attackCurve) / attackCurve) / numAttackPixels);
    attackOffset = (1 + attackCurve) * (1.0 - attackCoefficient);

    // Decay
    float numDecayPixels = convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decayRate, 0.5, 1000.0) * maxSegmentWidth;
    decayPoint.setX(attackPoint.getX() + numDecayPixels);
    decayPoint.setY(segmentView.getBottom() - sustainLevel * segmentView.getHeight());

    decayCoefficient = std::exp(-std::log((1.0 - sustainLevel + decayCurve) / decayCurve) / numDecayPixels);
    decayOffset = (sustainLevel  - decayCurve) * (1.0 - decayCoefficient);

    // Release
    float numReleasePixels = convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, releaseRate, 0.5, 1000.0) * maxSegmentWidth;
    releasePoint.setX(decayPoint.getX() + numReleasePixels);
    releasePoint.setY(segmentView.getBottom());

    releaseCoefficient = std::exp(-std::log((sustainLevel + releaseCurve) / releaseCurve) / numReleasePixels);
    releaseOffset = -releaseCurve * (1.0 - releaseCoefficient);
    
    repaint();
}
