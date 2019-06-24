/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ControlBarPanel.h"
#include "OscillatorPanel.h"
#include "OutputPanel.h"
#include "PluginProcessor.h"
#include "ScopePanel.h"

//==============================================================================
/**
*/
class MiniSynthAudioProcessorEditor : public AudioProcessorEditor
{
public:
    MiniSynthAudioProcessorEditor (MiniSynthAudioProcessor&);
    ~MiniSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MiniSynthAudioProcessor& processor;
    
    ControlBarPanel controlBarComponent;
    MidiKeyboardComponent midiKeyboardComponent;
    OutputPanel outputComponent;
    OscillatorPanel oscillatorComponent1;
    OscillatorPanel oscillatorComponent2;
    ScopePanel scopeComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniSynthAudioProcessorEditor)
};
