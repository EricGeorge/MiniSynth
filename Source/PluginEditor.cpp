/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "EnvelopeParameters.h"
#include "OscillatorParameters.h"
#include "PluginLayout.h"
#include "WavetableParameters.h"


//==============================================================================
MiniSynthAudioProcessorEditor::MiniSynthAudioProcessorEditor (MiniSynthAudioProcessor& processor)
:   AudioProcessorEditor (&processor),
    processor (processor),
    controlBarComponent(),
    envelopeComponent("Amp Envelope", envelope_ParamIDs),
    midiKeyboardComponent(processor.getMidiKeyboardstate(), MidiKeyboardComponent::horizontalKeyboard),
    scopeComponent(processor.getAudioBufferQueue()),
    wavetableComponent("Wavetable Oscillator", wavetable_ParamIDs, processor.getSynth().getSynthSound())
{
    addAndMakeVisible(controlBarComponent);
    addAndMakeVisible(envelopeComponent);
    addAndMakeVisible(wavetableComponent);
    addAndMakeVisible(scopeComponent);
    addAndMakeVisible(midiKeyboardComponent);

    setSize(pluginEditorWidth, pluginEditorHeight);
    
    midiKeyboardComponent.setKeyWidth(midiKeyboardKeyWidth);

    envelopeComponent.setupAttachments(processor.getValueTreeState());
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
    wavetableComponent.setBounds(oscillatorX, oscillatorY, oscillatorWidth, oscillatorHeight);
    envelopeComponent.setBounds(envelopeX, envelopeY, envelopeWidth, envelopeHeight);
    scopeComponent.setBounds(scopeX, scopeY, scopeWidth, scopeHeight);
    midiKeyboardComponent.setBounds(midiKeyboardX, midiKeyboardY, midiKeyboardWidth, midiKeyboardHeight);
}
