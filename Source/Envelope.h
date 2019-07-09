/*
  ==============================================================================

    Envelope.h
    Created: 6 Jul 2019 5:30:04pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

class Envelope
{
public:
    Envelope(double sampleRate);
    ~Envelope();
    
    enum State
    {
        Idle = 0,
        Attack,
        Decay,
        Sustain,
        Release
    };
    
    void start();
    void end();
    
    double getNextSampleMod();

    // parameter setters
    void setAttack(double newValue);
    void setDecay(double newValue);
    void setSustain(double newValue);
    void setRelease(double newValue);
    
    // getters
    State getState() const;
    
private:
    double sampleRate;
    
    State state;
    double output;
    
    // parameters
    double attack;
    double decay;
    double sustain;
    double release;
    
    double attackIncrement;
    double decayIncrement;
    double releaseIncrement;
    
    void update();
};

using Env = Envelope;
