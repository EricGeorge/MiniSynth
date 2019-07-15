/*
  ==============================================================================

    EnvelopeHelpers.h
    Created: 12 Jul 2019 12:49:32pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <cmath>

#include "EnvelopeParameters.h"

inline double calculateEnvCoefficient(double maxLevel, double timeConstant, double numSamples)
{
    return std::exp(-std::log((maxLevel + timeConstant) / timeConstant) / numSamples);
}

inline double calculateEnvOffset(double maxLevel, double timeConstant, double coefficient)
{
    return (maxLevel + timeConstant) * (1.0 - coefficient);
}

inline void writeEnvelopeToWaveFile(double attackRate,
                                    double decayRate,
                                    double sustainLevel,
                                    double releaseRate,
                                    double attackCurve,
                                    double decayCurve,
                                    double releaseCurve,
                                    double sampleRate)
{
    float attackNumSamples = attackRate / 1000.0 * sampleRate;
    double attackCoefficient = calculateEnvCoefficient(1.0, attackCurve, attackNumSamples);
    double attackOffset = calculateEnvOffset(1.0, attackCurve, attackCoefficient);

    float decayNumSamples = decayRate / 1000.0 * sampleRate;
    double decayCoefficient = calculateEnvCoefficient(1.0 - sustainLevel, decayCurve, decayNumSamples);
    double decayOffset = calculateEnvOffset(sustainLevel, -decayCurve, decayCoefficient);

    float releaseNumSamples = releaseRate / 1000.0 * sampleRate;
    double releaseCoefficient = calculateEnvCoefficient(sustainLevel, releaseCurve, releaseNumSamples);
    double releaseOffset = calculateEnvOffset(0.0, -releaseCurve, releaseCoefficient);
    
    std::vector<float> envelope;
    
    enum State
    {
        Idle = 0,
        Attack,
        Decay,
        Sustain,
        Release
    };
    
    State state = Attack;
    
    double output = 0;
    
    while (state != Idle)
    {
        switch(state)
        {
            case Attack:
                output = attackOffset + output * attackCoefficient;
                if (output >= 1.0)
                {
                    output = 1.0;
                    state = Decay;
                }
                break;
            case Decay:
                output = decayOffset + output * decayCoefficient;
                if (output <= sustainLevel)
                {
                    output = sustainLevel;
                    state = Release;
                }
                break;
            case Release:
                output = releaseOffset + output * releaseCoefficient;
                if (output <= 0.0)
                {
                    output = 0.0;
                    state = Idle;
                }
                break;
            case Idle:
            case Sustain:
                break;
        }
        
        envelope.push_back(output);
    }

    AudioSampleBuffer buffer(1, static_cast<int>(envelope.size()));
    for (int index = 0; index < envelope.size(); index ++)
    {
        buffer.setSample(0, index, envelope[index]);
    }

    WavAudioFormat format;
    String targetFolder = (File::getSpecialLocation(File::userHomeDirectory)).getFullPathName() + "/Downloads/Envelopes/";
    
    if (!File(targetFolder).exists())
    {
        File(targetFolder).createDirectory();
    }
    
    String currentTime = Time::getCurrentTime().toString(true, true);
    String fileName = "Envelope_" + currentTime + ".wav";
    File outputFile = File(targetFolder + fileName);
    if (outputFile.exists())
    {
        outputFile.deleteFile();
    }
    
    FileOutputStream* outputTo = outputFile.createOutputStream();
    std::unique_ptr<AudioFormatWriter> writer(format.createWriterFor(outputTo, sampleRate, 1, 16, NULL, 0));
    writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
