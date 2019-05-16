/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "PluginHelpers.h"
#include "SynthVoice.h"

//==============================================================================
NanoSynthAudioProcessor::NanoSynthAudioProcessor()
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
    outputGain.addParameterListeners(state);
}

NanoSynthAudioProcessor::~NanoSynthAudioProcessor()
{
    synth.removeParameterListeners(state);
    outputGain.removeParameterListeners(state);
}

AudioProcessorValueTreeState::ParameterLayout NanoSynthAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;
    Synth::createParameterLayout(layout);
    Gain::createParameterLayout(layout);
    return layout;
 }

//==============================================================================
const String NanoSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NanoSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NanoSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NanoSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NanoSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NanoSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NanoSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NanoSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String NanoSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void NanoSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NanoSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiKeyboardState.reset();
}

void NanoSynthAudioProcessor::releaseResources()
{
    midiKeyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NanoSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void NanoSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // clear any unused output channels that may contain garbage
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    midiKeyboardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    outputGain.process(buffer, buffer.getNumSamples());
    
    scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());
}

//==============================================================================
bool NanoSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NanoSynthAudioProcessor::createEditor()
{
    return new NanoSynthAudioProcessorEditor (*this);
}

//==============================================================================
void NanoSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Store an xml representation of our state.
    std::unique_ptr<XmlElement>xmlState(state.copyState().createXml());
    
    if (xmlState.get() != nullptr)
        copyXmlToBinary(*xmlState, destData);
}

void NanoSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore our plug-in's state from the xml representation stored in the above
    // method.
    std::unique_ptr<XmlElement>xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        state.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NanoSynthAudioProcessor();
}
