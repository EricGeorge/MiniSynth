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
    addAndMakeVisible(attackCurveSlider);
    addAndMakeVisible(decayCurveSlider);
    addAndMakeVisible(releaseCurveSlider);

    attackLabel.attachToComponent(&attackSlider, false);
    attackLabel.setFont(Font(sliderLabelFontSizeSmall));
    attackLabel.setColour(Label::textColourId, getCommonColours().detail);
    attackLabel.setJustificationType (Justification::centred);
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, attackSlider.getTextBoxWidth(), attackSlider.getTextBoxHeight());
    attackSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    attackSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    decayLabel.attachToComponent(&decaySlider, false);
    decayLabel.setFont(Font(sliderLabelFontSizeSmall));
    decayLabel.setColour(Label::textColourId, getCommonColours().detail);
    decayLabel.setJustificationType (Justification::centred);
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, decaySlider.getTextBoxWidth(), decaySlider.getTextBoxHeight());
    decaySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    decaySlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    sustainLabel.attachToComponent(&sustainSlider, false);
    sustainLabel.setFont(Font(sliderLabelFontSizeSmall));
    sustainLabel.setColour(Label::textColourId, getCommonColours().detail);
    sustainLabel.setJustificationType (Justification::centred);
    sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, sustainSlider.getTextBoxWidth(), sustainSlider.getTextBoxHeight());
    sustainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    sustainSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    releaseLabel.attachToComponent(&releaseSlider, false);
    releaseLabel.setFont(Font(sliderLabelFontSizeSmall));
    releaseLabel.setColour(Label::textColourId, getCommonColours().detail);
    releaseLabel.setJustificationType (Justification::centred);
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, releaseSlider.getTextBoxWidth(), releaseSlider.getTextBoxHeight());
    releaseSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    releaseSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    attackCurveLabel.attachToComponent(&attackCurveSlider, false);
    attackCurveLabel.setFont(Font(sliderLabelFontSizeSmall));
    attackCurveLabel.setColour(Label::textColourId, getCommonColours().detail);
    attackCurveLabel.setJustificationType (Justification::centred);
    attackCurveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, attackCurveSlider.getTextBoxWidth(), attackCurveSlider.getTextBoxHeight());
    attackCurveSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackCurveSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    attackCurveSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    decayCurveLabel.attachToComponent(&decayCurveSlider, false);
    decayCurveLabel.setFont(Font(sliderLabelFontSizeSmall));
    decayCurveLabel.setColour(Label::textColourId, getCommonColours().detail);
    decayCurveLabel.setJustificationType (Justification::centred);
    decayCurveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, decayCurveSlider.getTextBoxWidth(), decayCurveSlider.getTextBoxHeight());
    decayCurveSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decayCurveSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    decayCurveSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    releaseCurveLabel.attachToComponent(&releaseCurveSlider, false);
    releaseCurveLabel.setFont(Font(sliderLabelFontSizeSmall));
    releaseCurveLabel.setColour(Label::textColourId, getCommonColours().detail);
    releaseCurveLabel.setJustificationType (Justification::centred);
    releaseCurveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, releaseCurveSlider.getTextBoxWidth(), releaseCurveSlider.getTextBoxHeight());
    releaseCurveSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseCurveSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    releaseCurveSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    attackSlider.addListener(this);
    decaySlider.addListener(this);
    sustainSlider.addListener(this);
    releaseSlider.addListener(this);
    attackCurveSlider.addListener(this);
    decayCurveSlider.addListener(this);
    releaseCurveSlider.addListener(this);
}

EnvelopePanel::~EnvelopePanel()
{
    attackSlider.removeListener(this);
    decaySlider.removeListener(this);
    sustainSlider.removeListener(this);
    releaseSlider.removeListener(this);
    attackCurveSlider.removeListener(this);
    decayCurveSlider.removeListener(this);
    releaseCurveSlider.removeListener(this);
}

void EnvelopePanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    attackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Attack], attackSlider));
    decayAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Decay], decaySlider));
    sustainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Sustain], sustainSlider));
    releaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_Release], releaseSlider));
    attackCurveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_AttackCurve], attackCurveSlider));
    decayCurveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_DecayCurve], decayCurveSlider));
    releaseCurveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kEnvParam_ReleaseCurve], releaseCurveSlider));

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
    
    buttonsArea.reduced(8);
    buttonsArea.removeFromTop(40);
    buttonsArea.removeFromBottom(25);

    buttonsArea.removeFromLeft(envelopeKnobSpacing);
    attackCurveSlider.setBounds(buttonsArea.removeFromLeft(envelopeKnobWidth));

    buttonsArea.removeFromLeft(envelopeKnobSpacing);
    decayCurveSlider.setBounds(buttonsArea.removeFromLeft(envelopeKnobWidth));
    
    buttonsArea.removeFromLeft(envelopeKnobSpacing);
    releaseCurveSlider.setBounds(buttonsArea.removeFromLeft(envelopeKnobWidth));
    
//    interpolateButton.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth, wavetableKnobWidth / 2.5);
//
//    buttonsArea.removeFromLeft(wavetableKnobWidth + wavetableKnobSpacing);
//    exportWavetableButton.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth, wavetableKnobWidth / 2.5);
//
//    buttonsArea.removeFromLeft(wavetableKnobWidth + wavetableKnobSpacing);
//    wavetableSelector.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth * 3, wavetableKnobWidth / 2.5);
    
    sliderArea.reduced(8);
    sliderArea.removeFromTop(40);
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
                                      releaseSlider.getValue(),
                                      attackCurveSlider.getValue(),
                                      decayCurveSlider.getValue(),
                                      releaseCurveSlider.getValue());
}

