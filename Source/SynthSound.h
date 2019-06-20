/*
  ==============================================================================

    SynthSound.h
    Created: 19 Jun 2019 7:08:39pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Wavetable.h"

enum SynthSoundBroadcastMessages
{
    kSSBC_WavetableChanged = 0
};

static String synthSound_BroadcastIDs[]
{
    "Wavetable_Changed"
};


class SynthSound :  public SynthesiserSound,
                    public ActionBroadcaster
{
public:
    SynthSound();
    ~SynthSound();
    
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;
    
    void setWavetableFile(const String wavFile);
    
    Wavetable& getWavetable();
    
private:
    Wavetable wavetable;
    
    AudioFormatManager formatManager;
};
