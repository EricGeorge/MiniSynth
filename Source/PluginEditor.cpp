/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginLayout.h"


//==============================================================================
NanoSynthAudioProcessorEditor::NanoSynthAudioProcessorEditor (NanoSynthAudioProcessor& p)
:   AudioProcessorEditor (&p),
    processor (p),
    midiKeyboardComponent(processor.getMidiKeyboardstate(), MidiKeyboardComponent::horizontalKeyboard),
    scopeComponent(processor.getAudioBufferQueue())
{
    addAndMakeVisible(controlBarComponent);
    addAndMakeVisible(oscillatorComponent);
    addAndMakeVisible(outputComponent);
    addAndMakeVisible(scopeComponent);
    addAndMakeVisible(midiKeyboardComponent);

    setSize(pluginEditorWidth, pluginEditorHeight);
    
    midiKeyboardComponent.setKeyWidth(midiKeyboardKeyWidth);
    
    outputComponent.setupAttachments(p.getValueTreeState());
    oscillatorComponent.setupAttachments(p.getValueTreeState());
}

NanoSynthAudioProcessorEditor::~NanoSynthAudioProcessorEditor()
{
}

//==============================================================================
void NanoSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void NanoSynthAudioProcessorEditor::resized()
{
    controlBarComponent.setBounds(controlBarX, controlBarY, controlBarWidth, controlBarHeight);
    outputComponent.setBounds(outputX, outputY, outputWidth, outputHeight);
    oscillatorComponent.setBounds(oscillatorX, oscillatorY, oscillatorWidth, oscillatorHeight);
    scopeComponent.setBounds(scopeX, scopeY, scopeWidth, scopeHeight);
    midiKeyboardComponent.setBounds(midiKeyboardX, midiKeyboardY, midiKeyboardWidth, midiKeyboardHeight);
}
