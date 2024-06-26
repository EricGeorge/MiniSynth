/*
  ==============================================================================

    EnvelopeViewPanel.cpp
    Created: 9 Jul 2019 3:56:29pm
    Author:  Eric

  ==============================================================================
*/

#include "EnvelopeViewPanel.h"

#include "EnvelopeHelpers.h"
#include "EnvelopeParameters.h"
#include "PluginCommonStyling.h"
#include "PluginHelpers.h"

EnvelopeViewPanel::EnvelopeViewPanel()
:   segmentView(0, 0, 0, 0),
    attackPoint(0, 0),
    decayPoint(0, 0),
    releasePoint(0, 0)
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
    
    // put in segment lines
    float segmentDividerOpacity = 0.7f;
    
    g.setColour(Colours::ghostwhite);
    g.setOpacity(segmentDividerOpacity);
    g.drawLine(attackPoint.getX(), attackPoint.getY(), attackPoint.getX(), segmentView.getBottom());
    g.drawLine(decayPoint.getX(), decayPoint.getY(), decayPoint.getX(), segmentView.getBottom());

    // control dots
    int radius = 8;
    int diameter = 2 * radius;

    g.setColour(Colours::ghostwhite);
    g.setOpacity(segmentDividerOpacity);
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
                                        float sustainLevel,
                                        float releaseRate,
                                        float attackCurve,
                                        float decayCurve,
                                        float releaseCurve)
{
    float maxSegmentWidth = segmentView.getWidth() / 3;

    // Origin
    originPoint.setX(segmentView.getBottomLeft().getX());
    originPoint.setY(segmentView.getBottomLeft().getY());
    
    // Attack
    float numAttackPixels = convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attackRate, envAttackAnchorPoint, envAttackAnchorValue) * maxSegmentWidth;
    attackPoint.setX(segmentView.getX() + numAttackPixels);
    attackPoint.setY(segmentView.getY());
    
    double attackCoefficient = calculateEnvCoefficient(1.0, attackCurve, numAttackPixels);
    double attackOffset = calculateEnvOffset(1.0, attackCurve, attackCoefficient);

    // Decay
    float numDecayPixels = convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decayRate, envDecayAnchorPoint, envDecayAnchorValue) * maxSegmentWidth;
    decayPoint.setX(attackPoint.getX() + numDecayPixels);
    decayPoint.setY(segmentView.getBottom() - sustainLevel * segmentView.getHeight());

    double decayCoefficient = calculateEnvCoefficient(1.0 - sustainLevel, decayCurve, numDecayPixels);
    double decayOffset = calculateEnvOffset(sustainLevel, -decayCurve, decayCoefficient);

    // Release
    float numReleasePixels = convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, releaseRate, envReleaseAnchorPoint, envReleaseAnchorValue) * maxSegmentWidth;
    releasePoint.setX(decayPoint.getX() + numReleasePixels);
    releasePoint.setY(segmentView.getBottom());

    double releaseCoefficient = calculateEnvCoefficient(sustainLevel, releaseCurve, numReleasePixels);
    double releaseOffset = calculateEnvOffset(0.0, -releaseCurve, releaseCoefficient);

    // generate the new path:
    envelopePath.clear();
    envelopePath.startNewSubPath(originPoint);
    
    float envelope = 0;
    
    for (int pixel = originPoint.getX(); pixel < releasePoint.getX(); pixel++)
    {
        double offset = 0.0;
        double coefficient = 0.0;
        
        if (pixel <= attackPoint.getX())
        {
            offset = attackOffset;
            coefficient = attackCoefficient;
            envelope = offset + envelope * coefficient;
            if (envelope > 1.0)
            {
                envelope = 1.0;
            }
        }
        else if (pixel <= decayPoint.getX())
        {
            offset = decayOffset;
            coefficient = decayCoefficient;
            envelope = offset + envelope * coefficient;
            if (envelope < sustainLevel)
            {
                envelope = sustainLevel;
            }
        }
        else
        {
            offset = releaseOffset;
            coefficient = releaseCoefficient;
            envelope = offset + envelope * coefficient;
            if (envelope < 0.0)
            {
                envelope = 0.0;
            }
        }
        
        // correction for when segment points coincide
        if (pixel == static_cast<int>(decayPoint.getX()))
        {
            envelope = sustainLevel;
        }
        
        if (pixel == static_cast<int>(releasePoint.getX()))
        {
            envelope = 0.0;
        }
        
        envelopePath.lineTo(pixel, segmentView.getBottom() - envelope * segmentView.getHeight());
    }
    
    repaint();
}

