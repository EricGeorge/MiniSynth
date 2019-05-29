/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginLayout.h"


//==============================================================================
MiniSynthAudioProcessorEditor::MiniSynthAudioProcessorEditor (MiniSynthAudioProcessor& p)
:   AudioProcessorEditor (&p),
    processor (p),
    midiKeyboardComponent(processor.getMidiKeyboardstate(), MidiKeyboardComponent::horizontalKeyboard),
    scopeComponent(processor.getAudioBufferQueue())
{
    addAndMakeVisible(controlBarComponent);
    addAndMakeVisible(oscillatorComponent1);
    addAndMakeVisible(oscillatorComponent2);
    addAndMakeVisible(outputComponent);
    addAndMakeVisible(scopeComponent);
    addAndMakeVisible(midiKeyboardComponent);

    setSize(pluginEditorWidth, pluginEditorHeight);
    
    midiKeyboardComponent.setKeyWidth(midiKeyboardKeyWidth);
    
    outputComponent.setupAttachments(p.getValueTreeState());
    oscillatorComponent1.setupAttachments(p.getValueTreeState());
    oscillatorComponent2.setupAttachments(p.getValueTreeState());
}

MiniSynthAudioProcessorEditor::~MiniSynthAudioProcessorEditor()
{
}

//==============================================================================
void MiniSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MiniSynthAudioProcessorEditor::resized()
{
    controlBarComponent.setBounds(controlBarX, controlBarY, controlBarWidth, controlBarHeight);
    outputComponent.setBounds(outputX, outputY, outputWidth, outputHeight);
    oscillatorComponent1.setBounds(oscillatorX, oscillatorY, oscillatorWidth, oscillatorHeight / 2);
    oscillatorComponent2.setBounds(oscillatorX, oscillatorY + oscillatorHeight / 2 , oscillatorWidth, oscillatorHeight / 2);
    scopeComponent.setBounds(scopeX, scopeY, scopeWidth, scopeHeight);
    midiKeyboardComponent.setBounds(midiKeyboardX, midiKeyboardY, midiKeyboardWidth, midiKeyboardHeight);
}
