/*
  ==============================================================================

    WaveViewPanel.h
    Created: 18 Jun 2019 7:40:40pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "SynthSound.h"
#include "Wavetable.h"

class WaveViewPanel :  public Component
{
public:
    WaveViewPanel(SynthSound& sound);
    ~WaveViewPanel() override;
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void waveformChanged(float realFrameIndex);
    
private:
    // panel label
    const String panelName;
    
    SynthSound& sound;
    
    std::unique_ptr<BandLimitedWaveform> waveform1;
    int frameIndex1;
    
    std::unique_ptr<BandLimitedWaveform> waveform2;
    int frameIndex2;
    
    float interpFrac;    
};
