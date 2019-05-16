/*
  ==============================================================================

    Synth.cpp
    Created: 20 May 2019 10:21:21am
    Author:  Eric

  ==============================================================================
*/

#include "Synth.h"

#include "PluginHelpers.h"
#include "SynthVoice.h"

Synth::Synth()
{
    for (int i = 0; i < kNumVoices; ++i)
        addVoice(new SynthVoice());
    
    addSound(new SynthSound());
}

Synth::~Synth()
{
    
}

void Synth::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    SynthVoice::createParameterLayout(layout);
}

void Synth::addParameterListeners(AudioProcessorValueTreeState& state)
{
    for (int i = 0; i < getNumVoices(); ++i)
       dynamic_cast<SynthVoice*>(getVoice(i))->addParameterListeners(state);
}

void Synth::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    for (int i = 0; i < getNumVoices(); ++i)
        dynamic_cast<SynthVoice*>(getVoice(i))->removeParameterListeners(state);
}
