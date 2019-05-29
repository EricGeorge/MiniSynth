/*
  ==============================================================================

    OscillatorPanel.cpp
    Created: 20 May 2019 5:29:43pm
    Author:  Eric

  ==============================================================================
*/

#include "OscillatorPanel.h"

#include "PluginCommonStyling.h"
#include "OscillatorPanelLayout.h"
#include "OscillatorParameters.h"

OscillatorPanel::OscillatorPanel()
{
    addAndMakeVisible(wavetypeSlider);
    addAndMakeVisible(octavesSlider);
    addAndMakeVisible(semitonesSlider);
    addAndMakeVisible(centsSlider);
    addAndMakeVisible(pulseWidthSlider);
    addAndMakeVisible(polyBLEPMixSlider);
    addAndMakeVisible(waveShapeSaturationSlider);
    addAndMakeVisible(volumeSlider);

    wavetypeLabel.attachToComponent(&wavetypeSlider, true);
    wavetypeLabel.setFont(Font(sliderLabelFontSize));
    wavetypeLabel.setColour(Label::textColourId, getCommonColours().detail);
    wavetypeSlider.setTextBoxStyle(Slider::TextBoxRight, false, wavetypeSlider.getTextBoxWidth(), wavetypeSlider.getTextBoxHeight());
    wavetypeSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    wavetypeSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    octavesLabel.attachToComponent(&octavesSlider, true);
    octavesLabel.setFont(Font(sliderLabelFontSize));
    octavesLabel.setColour(Label::textColourId, getCommonColours().detail);
    octavesSlider.setTextBoxStyle(Slider::TextBoxRight, false, octavesSlider.getTextBoxWidth(), octavesSlider.getTextBoxHeight());
    octavesSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    octavesSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    semitonesLabel.attachToComponent(&semitonesSlider, true);
    semitonesLabel.setFont(Font(sliderLabelFontSize));
    semitonesLabel.setColour(Label::textColourId, getCommonColours().detail);
    semitonesSlider.setTextBoxStyle(Slider::TextBoxRight, false, semitonesSlider.getTextBoxWidth(), semitonesSlider.getTextBoxHeight());
    semitonesSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    semitonesSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    centsLabel.attachToComponent(&centsSlider, true);
    centsLabel.setFont(Font(sliderLabelFontSize));
    centsLabel.setColour(Label::textColourId, getCommonColours().detail);
    centsSlider.setTextBoxStyle(Slider::TextBoxRight, false, centsSlider.getTextBoxWidth(), centsSlider.getTextBoxHeight());
    centsSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    centsSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    pulseWidthLabel.attachToComponent(&pulseWidthSlider, true);
    pulseWidthLabel.setFont(Font(sliderLabelFontSize));
    pulseWidthLabel.setColour(Label::textColourId, getCommonColours().detail);
    pulseWidthSlider.setTextBoxStyle(Slider::TextBoxRight, false, pulseWidthSlider.getTextBoxWidth(), pulseWidthSlider.getTextBoxHeight());
    pulseWidthSlider.setTextValueSuffix("%");
    pulseWidthSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    pulseWidthSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    polyBLEPMixLabel.attachToComponent(&polyBLEPMixSlider, true);
    polyBLEPMixLabel.setFont(Font(sliderLabelFontSize));
    polyBLEPMixLabel.setColour(Label::textColourId, getCommonColours().detail);
    polyBLEPMixSlider.setTextBoxStyle(Slider::TextBoxRight, false, polyBLEPMixSlider.getTextBoxWidth(), polyBLEPMixSlider.getTextBoxHeight());
    polyBLEPMixSlider.setTextValueSuffix("%");
    polyBLEPMixSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    polyBLEPMixSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    waveShapeSaturationLabel.attachToComponent(&waveShapeSaturationSlider, true);
    waveShapeSaturationLabel.setFont(Font(sliderLabelFontSize));
    waveShapeSaturationLabel.setColour(Label::textColourId, getCommonColours().detail);
    waveShapeSaturationSlider.setTextBoxStyle(Slider::TextBoxRight, false, waveShapeSaturationSlider.getTextBoxWidth(), waveShapeSaturationSlider.getTextBoxHeight());
    waveShapeSaturationSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    waveShapeSaturationSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    volumeLabel.attachToComponent(&volumeSlider, true);
    volumeLabel.setFont(Font(sliderLabelFontSize));
    volumeLabel.setColour(Label::textColourId, getCommonColours().detail);
    volumeSlider.setTextBoxStyle(Slider::TextBoxRight, false, volumeSlider.getTextBoxWidth(), volumeSlider.getTextBoxHeight());
    volumeSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    volumeSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
}

OscillatorPanel::~OscillatorPanel()
{
    
}

void OscillatorPanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    wavetypeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorWavetype_ParameterID, wavetypeSlider));
    octavesAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorOctave_ParameterID, octavesSlider));
    semitonesAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorSemitone_ParameterID, semitonesSlider));
    centsAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorCents_ParameterID, centsSlider));
    pulseWidthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorPulseWidth_ParameterID, pulseWidthSlider));
    polyBLEPMixAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorPolyBLEPMix_ParameterID, polyBLEPMixSlider));
    waveShapeSaturationAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorWaveShapeSaturation_ParameterID, waveShapeSaturationSlider));
    volumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorVolume_ParameterID, volumeSlider));

    // setNumDecimalPlacesToDisplay doesn't seem to work so setup this lambda instead AFTER setting up the attachment!
    pulseWidthSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    pulseWidthSlider.updateText();
    
    polyBLEPMixSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    polyBLEPMixSlider.updateText();
    
    waveShapeSaturationSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    waveShapeSaturationSlider.updateText();
    
    volumeSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    volumeSlider.updateText();

}

void OscillatorPanel::resized()
{
    auto sliderArea = getLocalBounds().reduced(8);
    sliderArea.removeFromTop(50);
    sliderArea.removeFromLeft(120);
    sliderArea.removeFromRight(50);
    
    wavetypeSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    octavesSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    semitonesSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    centsSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    pulseWidthSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    polyBLEPMixSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    waveShapeSaturationSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    volumeSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
}

void OscillatorPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    String label = "OSCILLATOR";
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText(label, 0, 10, getWidth(), getHeight() - 10, Justification::centredTop);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}

