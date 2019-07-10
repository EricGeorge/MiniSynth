/*
  ==============================================================================

    EnvelopePanel.h
    Created: 9 Jul 2019 9:32:48am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "EnvelopeViewPanel.h"

class EnvelopePanel:    public Component,
                        public Slider::Listener
{
public:
    EnvelopePanel(const String panelName, const String* parameterList);
    ~EnvelopePanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setupAttachments(AudioProcessorValueTreeState& state);
    
    void sliderValueChanged(Slider* slider) override;
    
private:
    const String panelName;
    const String* parameterList;
    
    EnvelopeViewPanel envelopeViewPanel;
    
    // attack
    Label attackLabel{ {}, "ATTACK" };
    Slider attackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    
    // decay
    Label decayLabel{ {}, "DECAY" };
    Slider decaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    
    // sustain
    Label sustainLabel{ {}, "SUSTAIN" };
    Slider sustainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    
    // release
    Label releaseLabel{ {}, "RELEASE" };
    Slider releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
};
