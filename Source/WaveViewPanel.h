/*
  ==============================================================================

    WaveViewPanel.h
    Created: 18 Jun 2019 7:40:40pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class WaveViewPanel :  public Component
{
public:
    WaveViewPanel();
    ~WaveViewPanel() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    // panel label
    const String panelName;
};
