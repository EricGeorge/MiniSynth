/*
  ==============================================================================

    OutputComponent.cpp
    Created: 21 May 2019 10:33:23am
    Author:  Eric

  ==============================================================================
*/

#include "OutputPanel.h"

#include "OutputPanelLayout.h"
#include "OutputParameters.h"
#include "PluginCommonStyling.h"
#include "PluginHelpers.h"

OutputPanel::OutputPanel()
{
    addAndMakeVisible(outputGainSlider);

    outputGainSlider.setTextValueSuffix("dB");
    outputGainSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    outputGainSlider.setColour(Slider::ColourIds::backgroundColourId, getCommonColours().panelBackground);
}

OutputPanel::~OutputPanel()
{
    
}

void OutputPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    String label = "OUTPUT";
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText(label, 0, 10, getWidth(), getHeight() - 10, Justification::centredTop);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}

void OutputPanel::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(outputGainInsetTop);
    bounds.removeFromBottom(outputGainInsetBottom);
    bounds.removeFromRight(outputGainInsetLeft);
    bounds.removeFromLeft(outputGainInsetRight);
    outputGainSlider.setBounds(bounds);
}

void OutputPanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    outputGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, outputGain_ParameterID, outputGainSlider));
    
    outputGainSlider.textFromValueFunction = [](double value)
    {
        return juce::String(Decibels::gainToDecibels(value, kMinimumDecibels), 1);
    };
    outputGainSlider.updateText();

}
