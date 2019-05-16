/*
  ==============================================================================

    ControlBarComponent.h
    Created: 21 May 2019 10:32:49am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ControlBarPanel : public Component
{
public:
    ControlBarPanel();
    ~ControlBarPanel();
    
    void paint(Graphics& g) override;
    void resized() override;
};
