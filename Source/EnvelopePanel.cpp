/*
  ==============================================================================

    EnvelopePanel.cpp
    Created: 9 Jul 2019 9:32:48am
    Author:  Eric

  ==============================================================================
*/

#include "EnvelopePanel.h"

#include "EnvelopePanelLayout.h"
#include "EnvelopeParameters.h"
#include "PluginCommonStyling.h"


EnvelopePanel::EnvelopePanel(const String panelName, const String* parameterList)
:   panelName(panelName),
    parameterList(parameterList)
{
    
}

EnvelopePanel::~EnvelopePanel()
{
    
}

void EnvelopePanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    
}

void EnvelopePanel::resized()
{
    
}

void EnvelopePanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText(panelName, 0, 10, getWidth(), getHeight() - 10, Justification::centredTop);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}
