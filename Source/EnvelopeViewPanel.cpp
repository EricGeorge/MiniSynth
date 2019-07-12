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
    float numAttackPixels = convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attackRate, 0.5, 1000.0) * maxSegmentWidth;
    attackPoint.setX(segmentView.getX() + numAttackPixels);
    attackPoint.setY(segmentView.getY());
    
    double attackCoefficient = calculateEnvCoefficient(1.0, attackCurve, numAttackPixels);
    double attackOffset = calculateEnvOffset(1.0, attackCurve, attackCoefficient);

    // Decay
    float numDecayPixels = convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decayRate, 0.5, 1000.0) * maxSegmentWidth;
    decayPoint.setX(attackPoint.getX() + numDecayPixels);
    decayPoint.setY(segmentView.getBottom() - sustainLevel * segmentView.getHeight());

    double decayCoefficient = calculateEnvCoefficient(1.0 - sustainLevel, decayCurve, numDecayPixels);
    double decayOffset = calculateEnvOffset(sustainLevel, -decayCurve, decayCoefficient);

    // Release
    float numReleasePixels = convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, releaseRate, 0.5, 1000.0) * maxSegmentWidth;
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
        
        if (pixel < attackPoint.getX())
        {
            offset = attackOffset;
            coefficient = attackCoefficient;
        }
        else if (pixel < decayPoint.getX())
        {
            offset = decayOffset;
            coefficient = decayCoefficient;
        }
        else if (pixel < releasePoint.getX())
        {
            offset = releaseOffset;
            coefficient = releaseCoefficient;
        }
        
        // note - since we're always sloping to the sustain point, I'm not checking for sustain value to show in the display
        // if I were instead sloping to 0, then I would want to show sustain when decay was very linear.
        
        envelope = offset + envelope * coefficient;
        
        envelopePath.lineTo(pixel, segmentView.getBottom() - envelope * segmentView.getHeight());
    }
    
    repaint();
}
