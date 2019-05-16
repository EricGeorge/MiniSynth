/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioBufferQueue.h"
#include "Gain.h"
#include "ScopeDataCollector.h"
#include "Synth.h"

//==============================================================================
/**
*/
class NanoSynthAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    NanoSynthAudioProcessor();
    ~NanoSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    #ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif
    

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    AudioBufferQueue& getAudioBufferQueue() noexcept  { return audioBufferQueue; }
    AudioProcessorValueTreeState& getParameters() noexcept { return state; }
    MidiKeyboardState& getMidiKeyboardstate() noexcept { return midiKeyboardState; }
    AudioProcessorValueTreeState& getValueTreeState() noexcept { return state; }
 
    
private:
    //==============================================================================
    AudioBufferQueue audioBufferQueue;
    ScopeDataCollector scopeDataCollector { audioBufferQueue };
    
    MidiKeyboardState midiKeyboardState;
    
    AudioProcessorValueTreeState state;
    
    Gain outputGain;
    Synth synth;
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NanoSynthAudioProcessor)
};
