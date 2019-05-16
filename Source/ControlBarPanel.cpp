/*
  ==============================================================================

    ControlBarComponent.cpp
    Created: 21 May 2019 10:32:49am
    Author:  Eric

  ==============================================================================
*/

#include "ControlBarPanel.h"

#include "PluginCommonStyling.h"

ControlBarPanel::ControlBarPanel()
{
    
}

ControlBarPanel::~ControlBarPanel()
{
    
}

void ControlBarPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    String label = "CONTROLBAR";
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText(label, 0, 0, getWidth(), getHeight(), Justification::centred);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}

void ControlBarPanel::resized()
{
    
}

