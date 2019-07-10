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
    decayX(0),
    sustainY(0),
    releaseX(0),
    maxSegmentWidth(0),
    maxSegmentHeight(0)
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
    Rectangle<int> inset = border;
    inset.reduce(2, 2);
    g.setColour(getCommonColours().detailContrast);

    g.drawLine(Line<float>(inset.getX(), inset.getHeight(), inset.getX() + attackX, inset.getY()), 3.0);
    g.drawLine(Line<float>(inset.getX() + attackX, inset.getY(), inset.getX() + decayX, inset.getY() + (inset.getHeight() - sustainY)), 3.0);
    g.drawLine(Line<float>(inset.getX() + decayX, inset.getY() + (inset.getHeight() - sustainY), inset.getX() + releaseX, inset.getHeight()), 3.0);

    // paint the border
    g.setColour(getCommonColours().detail);
    g.drawRect(border, 2.0);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}

void EnvelopeViewPanel::resized()
{
    Rectangle<int> view = getLocalBounds().reduced(panelOutlineInset);
    float numPoints = view.getWidth();
    maxSegmentWidth = numPoints / 3;
    maxSegmentHeight = view.getHeight();
}

void EnvelopeViewPanel::envelopeChanged(float attack, float decay, float sustain, float release)
{
    attackX = maxSegmentWidth * convertFromRangeWithAnchor(envAttackMinValue, envAttackMaxValue, attack, 0.5, 1000.0);
    decayX = attackX + maxSegmentWidth * convertFromRangeWithAnchor(envDecayMinValue, envDecayMaxValue, decay, 0.5, 1000.0);
    sustainY = maxSegmentHeight * sustain;
    releaseX = decayX + maxSegmentWidth * convertFromRangeWithAnchor(envReleaseMinValue, envReleaseMaxValue, release, 0.5, 1000.0);
    
    repaint();
}
