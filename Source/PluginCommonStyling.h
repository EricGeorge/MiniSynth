/*
  ==============================================================================

    PluginCommonStyling.h
    Created: 23 May 2019 8:11:43pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//const Colour panelBackgroundColour = Colours::darkgrey;
//static const Colour detailColour(Colours::darkorange);

const int panelLabelFontSize = 26;
const int panelOutlineInset = 2;

const int sliderLabelFontSize = 20;

class CommonColours
{
public:
    CommonColours()
    :   panelBackground(Colours::darkgrey),
    detail(Colours::darkorange)
    {
    }
    
    const Colour panelBackground;
    const Colour detail;
};

static CommonColours& getCommonColours() { static CommonColours commonColours; return commonColours; }
