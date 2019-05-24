/*
  ==============================================================================

    BandLimitedOscillator.cpp
    Created: 24 May 2019 9:34:57am
    Author:  Eric

  ==============================================================================
*/

#include "BandLimitedOscillator.h"

#include "PluginHelpers.h"
#include "OscillatorBLEPTables.h"
#include "OscillatorHelpers.h"
#include "OscillatorParameters.h"

BandLimitedOscillator::BandLimitedOscillator(double sampleRate)
:   sampleRate(sampleRate),
    phaseInc(0.0),
    phase(0.0),
    phaseReset(false),
    modFrequency(0.0),
    dpwSquareModulator(-1.0),
    dpwRegister(0.0),
    waveType(PolyBLEPSawSquareMod),
    octaves(0),
    semitones(0),
    cents(0),
    pulseWidth(0.5f),
    polyBLEPMix(0.0f),
    waveShapeSaturation(1.0f),
    noteOn(false)
{
    seed = arc4random();
}

BandLimitedOscillator::~BandLimitedOscillator()
{
    
}

void BandLimitedOscillator::createParameterLayout(AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<AudioParameterInt>(oscillatorWaveform_ParameterID, "Wavetype", 1, 9, 1));
    layout.add(std::make_unique<AudioParameterInt>(oscillatorOctave_ParameterID, "Octave", -4, 4, 0));
    layout.add(std::make_unique<AudioParameterInt>(oscillatorSemitone_ParameterID, "Semitones", -12, 12, 0));
    layout.add(std::make_unique<AudioParameterInt>(oscillatorCents_ParameterID, "Cents", -100, 100, 0));
    layout.add(std::make_unique<AudioParameterFloat>(oscillatorPulseWidth_ParameterID, "Pulse Width", NormalisableRange<float> (1.0f, 99.0f), 50.0f));
    layout.add(std::make_unique<AudioParameterFloat>(oscillatorPolyBLEPMix_ParameterID, "PolyBLEP Mix", NormalisableRange<float> (0.0f, 100.0f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>(oscillatorWaveShapeSaturation_ParameterID, "Waveshape Saturation", NormalisableRange<float> (1.0f, 5.0f), 1.0f));
}

void BandLimitedOscillator::addParameterListeners(AudioProcessorValueTreeState& state)
{
    state.addParameterListener(oscillatorWaveform_ParameterID, this);
    state.addParameterListener(oscillatorOctave_ParameterID, this);
    state.addParameterListener(oscillatorSemitone_ParameterID, this);
    state.addParameterListener(oscillatorCents_ParameterID, this);
    state.addParameterListener(oscillatorPulseWidth_ParameterID, this);
    state.addParameterListener(oscillatorPolyBLEPMix_ParameterID, this);
    state.addParameterListener(oscillatorWaveShapeSaturation_ParameterID, this);
}

void BandLimitedOscillator::removeParameterListeners(AudioProcessorValueTreeState& state)
{
    state.removeParameterListener(oscillatorWaveform_ParameterID, this);
    state.removeParameterListener(oscillatorOctave_ParameterID, this);
    state.removeParameterListener(oscillatorSemitone_ParameterID, this);
    state.removeParameterListener(oscillatorCents_ParameterID, this);
    state.removeParameterListener(oscillatorPulseWidth_ParameterID, this);
    state.removeParameterListener(oscillatorPolyBLEPMix_ParameterID, this);
    state.removeParameterListener(oscillatorWaveShapeSaturation_ParameterID, this);
}

void BandLimitedOscillator::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == oscillatorWaveform_ParameterID)
    {
        waveType = static_cast<WaveType>(newValue);
    }
    else if (parameterID == oscillatorOctave_ParameterID)
    {
        octaves = static_cast<int>(newValue);
    }
    else if (parameterID == oscillatorSemitone_ParameterID)
    {
        semitones = static_cast<int>(newValue);
    }
    else if (parameterID == oscillatorCents_ParameterID)
    {
        cents = static_cast<int>(newValue);
    }
    else if (parameterID == oscillatorPulseWidth_ParameterID)
    {
        pulseWidth = newValue / 100.0f;
    }
    else if (parameterID == oscillatorPolyBLEPMix_ParameterID)
    {
        polyBLEPMix = newValue / 100.0f;
    }
    else if (parameterID == oscillatorWaveShapeSaturation_ParameterID)
    {
        waveShapeSaturation = newValue;
    }
}

void BandLimitedOscillator::reset(double sampleRate)
{
    sampleRate = sampleRate;
    modFrequency = 0.0;
    phaseInc = 0.0;
    phase = 0.0;
    phaseReset = false;
    dpwSquareModulator = -1.0;
    dpwRegister = 0.0;
    noteOn = false;
}

void BandLimitedOscillator::startNote(double frequency)
{
    modFrequency = frequency * getPitchFreqMod(octaves, semitones, cents);
    phaseInc = modFrequency / sampleRate;
    noteOn = true;
}

void BandLimitedOscillator::stopNote()
{
    noteOn = false;
    modFrequency = 0.0;
}

float BandLimitedOscillator::getNextSample()
{
    float sample = 0.0f;
    
    if (noteOn)
    {
        switch (waveType)
        {
            case BLEPSaw:
                sample = getNextBLEPSawSample();
                break;
            case PolyBLEPSawSquareMod:
                sample = getNextPolyBLEPSawSquareModSample();
                break;
            case UnipolarWaveShapedSaw:
                sample = getNextUnipolarWaveShapedSawSample();
                break;
            case BipolarWaveShapedSaw:
                sample = getNextBipolarWaveShapedSawSample();
                break;
            case SumofSawSquarePWM:
                sample = getNextSumOfSawSquarePWMSample();
                break;
            case DPWTriangle:
                sample = getNextDPWTriangleSample();
                break;
            case ParabolicSine:
                sample = getNextParabolicSineSample();
                break;
            case WhiteNoise:
                sample = getNextWhiteNoiseSample();
                break;
            case RandomNoise:
                sample = getNextRandomNoiseSample();
                break;
        }
        
        phase += phaseInc;
        if (phase > 1.0f)
        {
            phase -= 1.0f;
            phaseReset = true;
        }
        else
        {
            phaseReset = false;
        }
    }
    
    return sample;
}

float BandLimitedOscillator::getNextPolyBLEPSawSquareModSample()
{
    float alternatePhase = phase + 0.5f;
    if (alternatePhase > 1.0f)
        alternatePhase -= 1.0f;

    float sawSample =  unipolarToBipolar(phase) - polyBLEP(phase, phaseInc);
    
    float squareSample = phase > 0.5f ? 1.0f : -1.0f;
    squareSample += polyBLEP(alternatePhase, phaseInc) - polyBLEP(phase, phaseInc);
    
    // Mix the square and saw waves together
    return squareSample * polyBLEPMix + (sawSample * (1.0f - polyBLEPMix));
}

float BandLimitedOscillator::getNextBLEPSawSample()
{
    const double* blepTable = nullptr;
    size_t pointsPerSide = 0;
    const size_t tableLength = 4096;
    const size_t tableCenter = tableLength / 2 - 1;
    double blep = 0.0;
    
    if (modFrequency <= sampleRate / 8.0)
    {
        blepTable = &blepTable_8_BLKHAR[0];
        pointsPerSide = 4;
    }
    else
    {
        blepTable = &blepTable[0];
        pointsPerSide = 1;
    }
    
    // LEFT side of edge
    // -1 < t < 0
    for (int i = 1; i <= pointsPerSide; ++i)
    {
        if (phase > 1.0 - i * phaseInc)
        {
            double t = (phase - 1) / (pointsPerSide * phaseInc);
            float index = (1.0 + t) * tableCenter;
            float frac = index - int(index);
            blep = linear_interp(blepTable[(int)index], blepTable[(int)index + 1], frac);
        }
    }
    
    // RIGHT side of discontinuity
    // 0 <= t < 1
    for (int i = 1; i <= pointsPerSide; ++i)
    {
        if (phase < i * phaseInc)
        {
            double t = phase / (pointsPerSide * phaseInc);
            float index = t * tableCenter + (tableCenter + 1.0);
            float frac = index - int(index);
            blep = linear_interp(blepTable[(int)index], blepTable[(int)index + 1], frac);
        }
    }
    
    return unipolarToBipolar(phase) - blep;
}

float BandLimitedOscillator::getNextUnipolarWaveShapedSawSample()
{
    float waveShapedSawSample = tanh(waveShapeSaturation * phase) / tanh(waveShapeSaturation);
    return unipolarToBipolar(waveShapedSawSample) - polyBLEP(phase, phaseInc);
}

float BandLimitedOscillator::getNextBipolarWaveShapedSawSample()
{
    float trivialSawSample = unipolarToBipolar(phase);
    return tanh(waveShapeSaturation * trivialSawSample) / tanh(waveShapeSaturation) - polyBLEP(phase, phaseInc);
}

float BandLimitedOscillator::getNextSumOfSawSquarePWMSample()
{
    double phaseMod = phase;
    double saw1 = unipolarToBipolar(phase);
    
    if (phaseInc > 0)
        phaseMod += pulseWidth;
    else
        phaseMod -= pulseWidth;

    if (phaseInc > 0 && phaseMod >= 1.0)
        phaseMod -= 1.0;
    
    if (phaseInc < 0 && phaseMod <= 0)
        phaseMod += 1.0;
    
    double saw2 = unipolarToBipolar(phaseMod);
    double sample = 0.5 * saw1 - 0.5 * saw2;
    
    // DC Correction
    double correction = 1.0 / pulseWidth ;
    if (pulseWidth < 0.5)
        correction = 1.0 / (1.0 - pulseWidth);
    
    return sample *= correction;
}

float BandLimitedOscillator::getNextDPWTriangleSample()
{
    if(phaseReset)
        dpwSquareModulator *= -1.0;

    double sawSample = unipolarToBipolar(phase);
    double invertedSquaredSaw = 1.0 - sawSample * sawSample;
    double squareMod = invertedSquaredSaw * dpwSquareModulator;
    double differentiatedSquareMod = squareMod - dpwRegister;
    dpwRegister = squareMod;

    double c = sampleRate / (4.0 * 2.0 * modFrequency * (1 - phaseInc));
    return differentiatedSquareMod * c;
}

float BandLimitedOscillator::getNextParabolicSineSample()
{
    return (float)(parabolicSine(phase * 2.0 * pi - pi));
}

float BandLimitedOscillator::getNextWhiteNoiseSample()
{
    return unipolarToBipolar(arc4random_uniform(UINT32_MAX) / (double)UINT32_MAX);
}

float BandLimitedOscillator::getNextRandomNoiseSample()
{
    // get the bits
    unsigned int b0 = extractBits(seed, 1, 1); // 1 = b0 is FIRST bit from right
    unsigned int b1 = extractBits(seed, 2, 1); // 1 = b1 is SECOND bit from right
    unsigned int b27 = extractBits(seed, 28, 1); // 28 = b27 is 28th bit from right
    unsigned int b28 = extractBits(seed, 29, 1); // 29 = b28 is 29th bit from right
    
    // form the XOR
    unsigned int b31 = b0 ^ b1 ^ b27 ^ b28;
    
    // form the mask to OR with the register to load b31
    if(b31 == 1)
        b31 = 0x10000000;
    
    // shift one bit to right
    seed >>= 1;
    
    // set the b31 bit
    seed |= b31;
    
    // convert the output into a floating point number, scaled by experimentation
    // to a range of o to +2.0
    float sample = (float)(seed) / (pow(2.0f, 32.0f) / 16.0);
    
    // shift down to form a result from -1.0 to +1.0
    sample -= 1.0;
    
    return sample;
}
