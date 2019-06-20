/*
  ==============================================================================

    WavetablePanel.cpp
    Created: 17 Jun 2019 7:32:32pm
    Author:  Eric

  ==============================================================================
*/

#include "WavetablePanel.h"

#include "PluginCommonStyling.h"
#include "WavetablePanelLayout.h"
#include "WavetableParameters.h"

WavetablePanel::WavetablePanel(const String panelName, const String* parameterList, SynthSound& sound)
:   panelName(panelName),
    parameterList(parameterList),
    sound(sound)
{
    addAndMakeVisible(waveViewPanel);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(interpolateButton);
    addAndMakeVisible(exportWavetableButton);
    addAndMakeVisible(wavetableSelector);
    addAndMakeVisible(semitonesSlider);
    addAndMakeVisible(centsSlider);
    addAndMakeVisible(volumeSlider);
    
    positionLabel.attachToComponent(&positionSlider, false);
    positionLabel.setFont(Font(sliderLabelFontSize));
    positionLabel.setColour(Label::textColourId, getCommonColours().detail);
    positionLabel.setJustificationType (Justification::centred);
    positionSlider.setTextBoxStyle(Slider::TextBoxBelow, false, positionSlider.getTextBoxWidth(), positionSlider.getTextBoxHeight());
    positionSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    positionSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    positionSlider.setColour(Slider::thumbColourId, getCommonColours().detail);

    interpolateButton.setButtonText("Morph");
    interpolateButton.setClickingTogglesState(true);
    interpolateButton.setColour(TextButton::textColourOffId,  getCommonColours().detail);
    interpolateButton.setColour(TextButton::textColourOnId,   getCommonColours().panelBackground);
    interpolateButton.setColour(TextButton::buttonColourId,   getCommonColours().panelBackground);
    interpolateButton.setColour(TextButton::buttonOnColourId, getCommonColours().detail);
    interpolateButton.addListener(this);
    
    exportWavetableButton.setButtonText("Export");
    exportWavetableButton.setColour(TextButton::textColourOffId,  getCommonColours().detail);
    exportWavetableButton.setColour(TextButton::textColourOnId,   getCommonColours().panelBackground);
    exportWavetableButton.setColour(TextButton::buttonColourId,   getCommonColours().panelBackground);
    exportWavetableButton.setColour(TextButton::buttonOnColourId, getCommonColours().detail);
    exportWavetableButton.addListener(this);

    wavetableSelector.setText("Select Wavetable");
    wavetableSelector.setTextWhenNothingSelected("Select Wavetable");
    wavetableSelector.setColour(ComboBox::textColourId, getCommonColours().detail);
    wavetableSelector.setColour(ComboBox::backgroundColourId, getCommonColours().panelBackground);
    wavetableSelector.setColour(ComboBox::arrowColourId, getCommonColours().detail);
    wavetableSelector.setJustificationType(Justification::centred);
    wavetableSelector.addListener(this);
    
    semitonesLabel.attachToComponent(&semitonesSlider, false);
    semitonesLabel.setFont(Font(sliderLabelFontSize));
    semitonesLabel.setColour(Label::textColourId, getCommonColours().detail);
    semitonesLabel.setJustificationType (Justification::centred);
    semitonesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, semitonesSlider.getTextBoxWidth(), semitonesSlider.getTextBoxHeight());
    semitonesSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    semitonesSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    semitonesSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    centsLabel.attachToComponent(&centsSlider, false);
    centsLabel.setFont(Font(sliderLabelFontSize));
    centsLabel.setColour(Label::textColourId, getCommonColours().detail);
    centsLabel.setJustificationType (Justification::centred);
    centsSlider.setTextBoxStyle(Slider::TextBoxBelow, false, centsSlider.getTextBoxWidth(), centsSlider.getTextBoxHeight());
    centsSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    centsSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    centsSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    volumeLabel.attachToComponent(&volumeSlider, false);
    volumeLabel.setFont(Font(sliderLabelFontSize));
    volumeLabel.setColour(Label::textColourId, getCommonColours().detail);
    volumeLabel.setJustificationType (Justification::centred);
    volumeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, volumeSlider.getTextBoxWidth(), volumeSlider.getTextBoxHeight());
    volumeSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volumeSlider.setColour(Slider::textBoxTextColourId, getCommonColours().detail);
    volumeSlider.setColour(Slider::thumbColourId, getCommonColours().detail);
    
    populateWavetableSelector();
}

WavetablePanel::~WavetablePanel()
{
    interpolateButton.removeListener(this);
    exportWavetableButton.removeListener(this);
    wavetableSelector.removeListener(this);
}

void WavetablePanel::setupAttachments(AudioProcessorValueTreeState& state)
{
    positionAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kWtbParam_Position], positionSlider));
    interpolateAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(state, parameterList[kWtbParam_Interpolate], interpolateButton));
    semitonesAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kWtbParam_Semitones], semitonesSlider));
    centsAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kWtbParam_Cents], centsSlider));
    volumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(state, parameterList[kWtbParam_Volume], volumeSlider));
    
    // setNumDecimalPlacesToDisplay doesn't seem to work so setup this lambda instead AFTER setting up the attachment!
    centsSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    centsSlider.updateText();
    
    volumeSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    volumeSlider.updateText();
    
}

void WavetablePanel::buttonClicked (Button* b)
{
    if (b == &exportWavetableButton)
    {
        handleExportWavetableFrame();
    }
}

void WavetablePanel::comboBoxChanged(ComboBox* cb)
{
    if (wavetableSelector.getText() != wavetableSelector.getTextWhenNothingSelected())
    {
        String wavetableFilePath = (File::getSpecialLocation(File::userHomeDirectory)).getFullPathName() + wavetableFolderLocation + wavetableSelector.getText() + wavetableExtension;
        sound.setWavetableFile(wavetableFilePath);
    }
}

void WavetablePanel::resized()
{
    auto controlsArea = getLocalBounds().removeFromLeft(getLocalBounds().getWidth() / 2);
    auto waveViewArea = getLocalBounds().removeFromRight(controlsArea.getWidth());
    auto buttonsArea = controlsArea.removeFromTop(getLocalBounds().getHeight() * .5);
    auto sliderArea = controlsArea.removeFromBottom(buttonsArea.getHeight());

    waveViewPanel.setBounds(waveViewArea.reduced(40));

    buttonsArea.reduced(8);
    buttonsArea.removeFromTop(60);
    buttonsArea.removeFromLeft(wavetableKnobSpacing);
    interpolateButton.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth, wavetableKnobWidth / 2.5);

    buttonsArea.removeFromLeft(wavetableKnobWidth + wavetableKnobSpacing);
    exportWavetableButton.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth, wavetableKnobWidth / 2.5);

    buttonsArea.removeFromLeft(wavetableKnobWidth + wavetableKnobSpacing);
    wavetableSelector.setBounds(buttonsArea.getX(), buttonsArea.getY(), wavetableKnobWidth * 3, wavetableKnobWidth / 2.5);
    
    sliderArea.reduced(8);
    sliderArea.removeFromBottom(25);
    
    sliderArea.removeFromLeft(wavetableKnobSpacing);
    positionSlider.setBounds(sliderArea.removeFromLeft(wavetableKnobWidth));
    
    sliderArea.removeFromLeft(wavetableKnobSpacing * 2.5);
    semitonesSlider.setBounds(sliderArea.removeFromLeft(wavetableKnobWidth));
    
    sliderArea.removeFromLeft(wavetableKnobSpacing);
    centsSlider.setBounds(sliderArea.removeFromLeft(wavetableKnobWidth));
    
    sliderArea.removeFromLeft(wavetableKnobSpacing * 2.5);
    volumeSlider.setBounds(sliderArea.removeFromLeft(wavetableKnobWidth));
}

void WavetablePanel::paint(Graphics& g)
{
    Component::paint(g);
    
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    g.setColour(getCommonColours().detail);
    g.setFont(panelLabelFontSize);
    g.drawText(panelName, 0, 10, getWidth(), getHeight() - 10, Justification::centredTop);
    
    g.drawRect(getLocalBounds().reduced(panelOutlineInset));
}

void WavetablePanel::populateWavetableSelector()
{
    String wavetableFolder = (File::getSpecialLocation(File::userHomeDirectory)).getFullPathName() + wavetableFolderLocation;
    
    int comboBoxIndex = 1;
    std::vector<File> wavetables;
    for(DirectoryIterator di(File(wavetableFolder),
                             false,
                             "*" + wavetableExtension,
                             File::TypesOfFileToFind::findFiles); di.next();)
    {
        File wavetable = di.getFile();
        wavetables.push_back(wavetable);
    }
    
    sort(wavetables.begin(), wavetables.end(),
         [](const File & a, const File & b) -> bool
         {
             return a.getFileNameWithoutExtension() < b.getFileNameWithoutExtension();
         });
    
    std::for_each(wavetables.begin(),
                  wavetables.end(),
                  [this, comboBoxIndex](File &wavetable) mutable { wavetableSelector.addItem(wavetable.getFileNameWithoutExtension(), comboBoxIndex++); });
}

void WavetablePanel::handleExportWavetableFrame()
{
    if (wavetableSelector.getText() != wavetableSelector.getTextWhenNothingSelected())
    {
        sound.getWavetable().WriteFrameToWaveFile(wavetableSelector.getText() + "_Frame_0.wav", 0);
    }
}
