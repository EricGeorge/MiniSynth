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
:   attackX(0),
    attackY(0),
    decayX(0),
    sustainY(0),
    releaseX(0),
    releaseY(0),
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
    
    Rectangle<int> border(getLocalBounds().reduced(panelOutlineInset));

    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    // paint the envelope
    g.setColour(getCommonColours().detailContrast);

    Path envelopePath;
    envelopePath.startNewSubPath(segmentView.getX(), segmentView.getBottom());
    envelopePath.lineTo(attackX, segmentView.getY());
    envelopePath.lineTo(decayX, sustainY);
    envelopePath.lineTo(releaseX, segmentView.getBottom());
    
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
    
    // paint the border
    g.setColour(getCommonColours().detail);
    g.drawRect(border, 2.0);
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
    attackX =  segmentView.getX() + convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attack, 0.5, 1000.0) * maxSegmentWidth;
    decayX = attackX + convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decay, 0.5, 1000.0) * maxSegmentWidth;
    sustainY = segmentView.getBottom() - segmentView.getHeight() * sustain;
    releaseX = decayX + convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, release, 0.5, 1000.0) * maxSegmentWidth;
    
    repaint();
}
