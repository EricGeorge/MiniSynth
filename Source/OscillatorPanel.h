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
    OscillatorPanel(const String panelName, const String* parameterList, const bool* parameterEnables);
    ~OscillatorPanel() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setupAttachments(AudioProcessorValueTreeState& state);
    
private:
    // panel label
    const String panelName;
    const String* parameterList;
    
    // waveform
    Label wavetypeLabel { {}, "Waveform:" };
    Slider wavetypeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wavetypeAttachment;
    
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
    
    // polyBLEP mix
    Label polyBLEPMixLabel { {}, "PolyBLEP mix:" };
    Slider polyBLEPMixSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> polyBLEPMixAttachment;
    
    // waveshape saturation
    Label waveShapeSaturationLabel { {}, "Saturation:" };
    Slider waveShapeSaturationSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waveShapeSaturationAttachment;
    
    // volume
    Label volumeLabel { {}, "Volume:" };
    Slider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
};
