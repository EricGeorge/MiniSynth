/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthHelpers.h"
#include "SynthVoice.h"

//==============================================================================
BasicJuceSynthAudioProcessor::BasicJuceSynthAudioProcessor()
{
    for (int i = 0; i < kNumVoices; ++i)
        synth.addVoice(new SynthVoice());
    
    synth.addSound(new SynthSound());
}

BasicJuceSynthAudioProcessor::~BasicJuceSynthAudioProcessor()
{
}

//==============================================================================
const String BasicJuceSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicJuceSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicJuceSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicJuceSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicJuceSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicJuceSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicJuceSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicJuceSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicJuceSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicJuceSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BasicJuceSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void BasicJuceSynthAudioProcessor::releaseResources()
{
}

void BasicJuceSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool BasicJuceSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BasicJuceSynthAudioProcessor::createEditor()
{
    return new BasicJuceSynthAudioProcessorEditor (*this);
}

//==============================================================================
void BasicJuceSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicJuceSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicJuceSynthAudioProcessor();
}
