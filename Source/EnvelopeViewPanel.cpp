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
    segmentView(0, 0, 0, 0)
{
    
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
    envelopePath.lineTo(attackPoint);
    envelopePath.lineTo(decayPoint);
    envelopePath.lineTo(releasePoint);
    
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

void EnvelopeViewPanel::envelopeChanged(float attack, float decay, float sustain, float release)
{
    originPoint.setX(segmentView.getBottomLeft().getX());
    originPoint.setY(segmentView.getBottomLeft().getY());
    
    attackPoint.setX(segmentView.getX() + convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attack, 0.5, 1000.0) * maxSegmentWidth);
    attackPoint.setY(segmentView.getY());

    decayPoint.setX(attackPoint.getX() + convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decay, 0.5, 1000.0) * maxSegmentWidth);
    decayPoint.setY(segmentView.getBottom() - sustain * segmentView.getHeight());

    releasePoint.setX(decayPoint.getX() + convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, release, 0.5, 1000.0) * maxSegmentWidth);
    releasePoint.setY(segmentView.getBottom());
    
    repaint();
}
