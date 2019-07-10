/*
  ==============================================================================

    EnvelopePanel.cpp
    Created: 9 Jul 2019 9:32:48am
    Author:  Eric

  ==============================================================================
*/

#include "EnvelopePanel.h"

#include "EnvelopePanelLayout.h"
#include "EnvelopeParameters.h"
#include "PluginCommonStyling.h"


EnvelopePanel::EnvelopePanel(const String panelName, const String* parameterList)
:   panelName(panelName),
    parameterList(parameterList)
{
    addAndMakeVisible(envelopeViewPanel);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
    
    attackLabel.attachToComponent(&attackSlider, false);
    attackLabel.setFont(Font(sliderLabelFontSize));
    attackLabel.setColour(Label::textColourId, getCommonColours().detail);
    attackLabel.setJustificationType (Justification::centred);
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, attackSlider.getTextBoxWidth(), attackSlider.getTextBoxHeight());
    attackSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    attackSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    decayLabel.attachToComponent(&decaySlider, false);
    decayLabel.setFont(Font(sliderLabelFontSize));
    decayLabel.setColour(Label::textColourId, getCommonColours().detail);
    decayLabel.setJustificationType (Justification::centred);
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, decaySlider.getTextBoxWidth(), decaySlider.getTextBoxHeight());
    decaySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    decaySlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    sustainLabel.attachToComponent(&sustainSlider, false);
    sustainLabel.setFont(Font(sliderLabelFontSize));
    sustainLabel.setColour(Label::textColourId, getCommonColours().detail);
    sustainLabel.setJustificationType (Justification::centred);
    sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, sustainSlider.getTextBoxWidth(), sustainSlider.getTextBoxHeight());
    sustainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    sustainSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    releaseLabel.attachToComponent(&releaseSlider, false);
    releaseLabel.setFont(Font(sliderLabelFontSize));
    releaseLabel.setColour(Label::textColourId, getCommonColours().detail);
    releaseLabel.setJustificationType (Justification::centred);
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, releaseSlider.getTextBoxWidth(), releaseSlider.getTextBoxHeight());
    releaseSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    releaseSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    attackSlider.addListener(this);
    decaySlider.addListener(this);
    sustainSlider.addListener(this);
    releaseSlider.addListener(this);
}

EnvelopePanel::~EnvelopePanel()
{
    attackSlider.removeListener(this);
    decaySlider.removeListener(this);
    sustainSlider.removeListener(this);
    releaseSlider.removeListener(this);
}

void EnvelopePanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    attackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Attack], attackSlider));
    decayAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Decay], decaySlider));
    sustainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Sustain], sustainSlider));
    releaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Release], releaseSlider));
    
    // setNumDecimalPlacesToDisplay doesn't seem to work so setup this lambda instead AFTER setting up the attachment!
    attackSlider.textFromValueFunction = [this](double value)
    {
        String units = ms;
        int precision = 1;
        if (value > 1000.0)
        {
            value = value / 1000.0;
            units = s;
            precision = 2;
        }
        return juce::String(value, precision) + units;
    };
    attackSlider.updateText();
    
    decaySlider.textFromValueFunction = [this](double value)
    {
        String units = ms;
        int precision = 1;
        if (value > 1000.0)
        {
            value = value / 1000.0;
            units = s;
            precision = 2;
        }
        return juce::String(value, precision) + units;
    };
    decaySlider.updateText();
    
    sustainSlider.textFromValueFunction = [this](double value)
    {
        return juce::String(value, 2);
    };
    sustainSlider.updateText();
    
    releaseSlider.textFromValueFunction = [this](double value)
    {
        String units = ms;
        int precision = 1;
        if (value > 1000.0)
        {
            value = value / 1000.0;
            units = s;
            precision = 2;
        }
        return juce::String(value, precision) + units;
    };
    releaseSlider.updateText();
}

void EnvelopePanel::resized()
{
    auto controlsArea = getLocalBounds().removeFromLeft(getLocalBounds().getWidth() * .4);
    auto envelopeViewArea = getLocalBounds().removeFromRight(getLocalBounds().getWidth() - controlsArea.getWidth());
    auto buttonsArea = controlsArea.removeFromTop(getLocalBounds().getHeight() * .5);
    auto sliderArea = controlsArea.removeFromBottom(buttonsArea.getHeight());
    
    envelopeViewPanel.setBounds(envelopeViewArea.reduced(40));
    
//    buttonsArea.reduced(8);
//    buttonsArea.removeFromTop(60);
//    buttonsArea.removeFromLeft(wavetableKnobSpacing);
//    interpolateButton.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth, wavetableKnobWidth / 2.5);
//
//    buttonsArea.removeFromLeft(wavetableKnobWidth + wavetableKnobSpacing);
//    exportWavetableButton.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth, wavetableKnobWidth / 2.5);
//
//    buttonsArea.removeFromLeft(wavetableKnobWidth + wavetableKnobSpacing);
//    wavetableSelector.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth * 3, wavetableKnobWidth / 2.5);
    
    sliderArea.reduced(8);
    sliderArea.removeFromBottom(25);
    
    sliderArea.removeFromLeft(envelopeKnobSpacing);
    attackSlider.setBounds(sliderArea.removeFromLeft(envelopeKnobWidth));
    
    sliderArea.removeFromLeft(envelopeKnobSpacing);
    decaySlider.setBounds(sliderArea.removeFromLeft(envelopeKnobWidth));
    
    sliderArea.removeFromLeft(envelopeKnobSpacing);
    sustainSlider.setBounds(sliderArea.removeFromLeft(envelopeKnobWidth));
    
    sliderArea.removeFromLeft(envelopeKnobSpacing);
    releaseSlider.setBounds(sliderArea.removeFromLeft(envelopeKnobWidth));
}

void EnvelopePanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText(panelName, 0, 10, getWidth(), getHeight() - 10, Justification::centredTop);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}

void EnvelopePanel::sliderValueChanged(Slider* slider)
{
    envelopeViewPanel.envelopeChanged(attackSlider.getValue(),
                                      decaySlider.getValue(),
                                      sustainSlider.getValue(),
                                      releaseSlider.getValue());
}

