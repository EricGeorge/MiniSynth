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
    const String ms = " ms";
    const String s = " s";
    
    EnvelopeViewPanel envelopeViewPanel;
    
    // attack
    Label attackLabel{ {}, "Attack" };
    Slider attackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    
    // decay
    Label decayLabel{ {}, "Decay" };
    Slider decaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    
    // sustain
    Label sustainLabel{ {}, "Sustain" };
    Slider sustainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    
    // release
    Label releaseLabel{ {}, "Release" };
    Slider releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    // attack curve
    Label attackCurveLabel{ {}, "Attack Curve" };
    Slider attackCurveSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackCurveAttachment;
    
    // decay curve
    Label decayCurveLabel{ {}, "Decay Curve" };
    Slider decayCurveSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayCurveAttachment;
    
    // release curve
    Label releaseCurveLabel{ {}, "Release Curve" };
    Slider releaseCurveSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseCurveAttachment;
};
