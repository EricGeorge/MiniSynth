/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "OscillatorParameters.h"
#include "PluginLayout.h"
#include "WavetableParameters.h"


//==============================================================================
MiniSynthAudioProcessorEditor::MiniSynthAudioProcessorEditor (MiniSynthAudioProcessor& processor)
:   AudioProcessorEditor (&processor),
    processor (processor),
    midiKeyboardComponent(processor.getMidiKeyboardstate(), MidiKeyboardComponent::horizontalKeyboard),
    oscillatorComponent("VA Oscillator", oscillator_ParamIDs),
    wavetableComponent("Wavetable Oscillator", wavetable_ParamIDs, processor.getSynth().getSynthSound()),
    scopeComponent(processor.getAudioBufferQueue())
{
    addAndMakeVisible(controlBarComponent);
    addAndMakeVisible(oscillatorComponent);
    addAndMakeVisible(wavetableComponent);
    addAndMakeVisible(outputComponent);
    addAndMakeVisible(scopeComponent);
    addAndMakeVisible(midiKeyboardComponent);

    setSize(pluginEditorWidth, pluginEditorHeight);
    
    midiKeyboardComponent.setKeyWidth(midiKeyboardKeyWidth);
    
    outputComponent.setupAttachments(processor.getValueTreeState());
    oscillatorComponent.setupAttachments(processor.getValueTreeState());
    wavetableComponent.setupAttachments(processor.getValueTreeState());
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
    oscillatorComponent.setBounds(oscillatorX, oscillatorY, oscillatorWidth, oscillatorHeight / 2);
    wavetableComponent.setBounds(oscillatorX, oscillatorY + oscillatorHeight / 2 , oscillatorWidth, oscillatorHeight / 2);
    scopeComponent.setBounds(scopeX, scopeY, scopeWidth, scopeHeight);
    midiKeyboardComponent.setBounds(midiKeyboardX, midiKeyboardY, midiKeyboardWidth, midiKeyboardHeight);
}
