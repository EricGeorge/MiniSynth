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
    void setAttackCurve(double newValue);
    void setDecayCurve(double newValue);
    void setReleaseCurve(double newValue);
    
    // getters
    State getState() const;
    
private:
    double sampleRate;
    
    State state;
    double output;
    
    // parameters
    double attackRate;
    double decayRate;
    double sustainLevel;
    double releaseRate;
    double attackCurve;
    double decayCurve;
    double releaseCurve;
    
    double attackCoefficient;
    double attackOffset;
    
    double decayCoefficient;
    double decayOffset;
    
    double releaseCoefficient;
    double releaseOffset;
};

using Env = Envelope;
