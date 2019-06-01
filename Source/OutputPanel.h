/*
  ==============================================================================

    OutputComponent.h
    Created: 21 May 2019 10:33:23am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OutputPanel : public Component
{
public:
    OutputPanel();
    ~OutputPanel() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setupAttachments(AudioProcessorValueTreeState& state);
    
private:
    Label outputGainLabel { {}, "Output Gain:" };
    Slider outputGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;

};
