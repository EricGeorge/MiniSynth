/*
  ==============================================================================

    WaveViewPanel.cpp
    Created: 18 Jun 2019 7:40:40pm
    Author:  Eric

  ==============================================================================
*/

#include "WaveViewPanel.h"

#include "PluginCommonStyling.h"

WaveViewPanel::WaveViewPanel()
{
    
}

WaveViewPanel::~WaveViewPanel()
{
    
}

void WaveViewPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText("WaveView Panel", 0, 10, getWidth(), getHeight() - 10, Justification::centredTop);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));

}

void WaveViewPanel::resized()
{
    
}
