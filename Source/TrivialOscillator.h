/*
  ==============================================================================

    NaiveOscillator.h
    Created: 13 May 2019 8:15:21pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

enum Waveform {
    Waveform_Sine,
    Waveform_Saw,
    Waveform_Triangle,
    Waveform_Square
};

class TrivialOscillator
{
public:
    TrivialOscillator(double sampleRate);
    ~TrivialOscillator();
    
    void reset(double sampleRate);
    
    void startNote(double frequency);
    void stopNote();
    
    float getNextSample();
    
private:
    double sampleRate;
    double phaseInc;
    double phase;
    
    Waveform waveform;
    
    // poor man's envelope - replace with ADSR
    bool noteOn;
};
