/*
  ==============================================================================

    WavetablePanel.h
    Created: 17 Jun 2019 7:32:32pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "WaveViewPanel.h"

#include "SynthSound.h"

const static String wavetableFolderLocation = "/Library/Audio/Presets/yourcompany/MiniSynth/Tables/";
const static String wavetableExtension = ".wav";

class WavetablePanel :  public Component,
                        public Button::Listener,
                        public ComboBox::Listener
{
public:
    WavetablePanel(const String panelName, const String* parameterList, SynthSound& sound);
    ~WavetablePanel() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setupAttachments(AudioProcessorValueTreeState& state);
    
    void buttonClicked (Button* b) override;
    void comboBoxChanged(ComboBox* cb) override;
    
    void handleExportWavetableFrame();


private:
    // panel label
    const String panelName;
    const String* parameterList;
    SynthSound& sound;
    
    WaveViewPanel waveViewPanel;
    
    // position
    Label positionLabel{ {}, "POSITION" };
    Slider positionSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionAttachment;
    
    // interpolate
    TextButton interpolateButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> interpolateAttachment;
    
    // export wavetable
    TextButton exportWavetableButton;
    
    // wavetable
    ComboBox wavetableSelector;
    
    // semitones
    Label semitonesLabel { {}, "COARSE" };
    Slider semitonesSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> semitonesAttachment;
    
    // cents
    Label centsLabel { {}, "FINE" };
    Slider centsSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centsAttachment;
    
    // volume
    Label volumeLabel { {}, "VOLUME" };
    Slider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    void populateWavetableSelector();
};
