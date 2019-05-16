/*
  ==============================================================================

    OscillatorPanel.h
    Created: 20 May 2019 5:29:43pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OscillatorPanel : public Component
{
public:
    OscillatorPanel();
    ~OscillatorPanel();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setupAttachments(AudioProcessorValueTreeState& state);
    
private:
    // waveform
    Label waveformLabel { {}, "Waveform:" };
    Slider waveformSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waveformAttachment;
    
    // octaves
    Label semitonesLabel { {}, "Semitones:" };
    Slider semitonesSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> semitonesAttachment;
    
    // semitones
    Label octavesLabel { {}, "Octaves:" };
    Slider octavesSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> octavesAttachment;
    
    // cents
    Label centsLabel { {}, "Cents:" };
    Slider centsSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centsAttachment;
    
    // pulse width
    Label pulseWidthLabel { {}, "Pulse width:" };
    Slider pulseWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pulseWidthAttachment;
    
};
