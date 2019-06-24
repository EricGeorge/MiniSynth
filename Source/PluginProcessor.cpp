/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "PluginHelpers.h"

//==============================================================================
MiniSynthAudioProcessor::MiniSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
            .withInput  ("Input",  AudioChannelSet::stereo(), true)
        #endif
        .withOutput ("Output", AudioChannelSet::stereo(), true)
    #endif
    ),
    state(*this,
          nullptr,
          juce::Identifier("PARAMETERS"),
          createParameterLayout())
#endif
{
    synth.addParameterListeners(state);
}

MiniSynthAudioProcessor::~MiniSynthAudioProcessor()
{
    synth.removeParameterListeners(state);
}

AudioProcessorValueTreeState::ParameterLayout MiniSynthAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;
    synth.createParameterLayout(layout);
    return layout;
 }

//==============================================================================
const String MiniSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MiniSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MiniSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MiniSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MiniSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MiniSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MiniSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MiniSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String MiniSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void MiniSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MiniSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiKeyboardState.reset();
}

void MiniSynthAudioProcessor::releaseResources()
{
    midiKeyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MiniSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void MiniSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // clear any unused output channels that may contain garbage
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    midiKeyboardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());
}

//==============================================================================
bool MiniSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MiniSynthAudioProcessor::createEditor()
{
    return new MiniSynthAudioProcessorEditor (*this);
}

//==============================================================================
void MiniSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Store an xml representation of our state.
    std::unique_ptr<XmlElement>xmlState(state.copyState().createXml());
    
    if (xmlState.get() != nullptr)
        copyXmlToBinary(*xmlState, destData);
}

void MiniSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore our plug-in's state from the xml representation stored in the above
    // method.
    std::unique_ptr<XmlElement>xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        state.replaceState(ValueTree::fromXml(*xmlState));
}

Synth& MiniSynthAudioProcessor::getSynth()
{
    return synth;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MiniSynthAudioProcessor();
}

