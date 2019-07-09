/*
  ==============================================================================

    EnvelopePanel.h
    Created: 9 Jul 2019 9:32:48am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopePanel: public Component
{
public:
    EnvelopePanel(const String panelName, const String* parameterList);
    ~EnvelopePanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setupAttachments(AudioProcessorValueTreeState& state);
    
private:
    const String panelName;
    const String* parameterList;
};
