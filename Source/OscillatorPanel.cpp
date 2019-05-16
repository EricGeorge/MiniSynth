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
    addAndMakeVisible(waveformSlider);
    addAndMakeVisible(octavesSlider);
    addAndMakeVisible(semitonesSlider);
    addAndMakeVisible(centsSlider);
    addAndMakeVisible(pulseWidthSlider);

    waveformLabel.attachToComponent(&waveformSlider, true);
    waveformLabel.setFont(Font(sliderLabelFontSize));
    waveformLabel.setColour(Label::textColourId, getCommonColours().detail);
    waveformSlider.setTextBoxStyle(Slider::TextBoxRight, false, waveformSlider.getTextBoxWidth(), waveformSlider.getTextBoxHeight());
    waveformSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    waveformSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

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
}

OscillatorPanel::~OscillatorPanel()
{
    
}

void OscillatorPanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    waveformAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorWaveform_ParameterID, waveformSlider));
    octavesAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorOctave_ParameterID, octavesSlider));
    semitonesAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorSemitone_ParameterID, semitonesSlider));
    centsAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorCents_ParameterID, centsSlider));
    pulseWidthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, oscillatorPulseWidth_ParameterID, pulseWidthSlider));
    
    // setNumDecimalPlacesToDisplay doesn't seem to work so setup this lambda instead AFTER setting up the attachment!
    pulseWidthSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    pulseWidthSlider.updateText();
}

void OscillatorPanel::resized()
{
    auto sliderArea = getLocalBounds().reduced(8);
    sliderArea.removeFromTop(50);
    sliderArea.removeFromLeft(120);
    sliderArea.removeFromRight(getWidth() / 2);
    
    waveformSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    octavesSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    semitonesSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    centsSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
    sliderArea.removeFromTop(oscillatorSliderSpacing);
    pulseWidthSlider.setBounds(sliderArea.removeFromTop(oscillatorSliderHeight));
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

