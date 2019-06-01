/*
  ==============================================================================

    BandLimitedOscillator.cpp
    Created: 24 May 2019 9:34:57am
    Author:  Eric

  ==============================================================================
*/

#include "BandLimitedOscillator.h"

#include <cstdlib>
#include <cstdint>

#include "OscillatorBLEPTables.h"
#include "OscillatorHelpers.h"


BandLimitedOscillator::BandLimitedOscillator(double sampleRate)
:   sampleRate(sampleRate),
    modFrequency(0.0),
    dpwDifferentiator(0.0),
    waveType(PolyBLEPSawSquareMod),
    octaves(0),
    semitones(0),
    cents(0),
    pulseWidth(0.5),
    polyBLEPMix(0.0),
    waveShapeSaturation(1.0),
    volume(1.0),
    noteOn(false)
{
    seed = arc4random();
}

BandLimitedOscillator::~BandLimitedOscillator()
{
    
}

void BandLimitedOscillator::setWaveType(float newValue)
{
    waveType = static_cast<WaveType>(newValue);
    reset(sampleRate);
}

void BandLimitedOscillator::setOctaves(float newValue)
{
    octaves = static_cast<int>(newValue);
}

void BandLimitedOscillator::setSemitones(float newValue)
{
    semitones = static_cast<int>(newValue);
}

void BandLimitedOscillator::setCents(float newValue)
{
    cents = static_cast<int>(newValue);
}

void BandLimitedOscillator::setPulseWidth(float newValue)
{
    pulseWidth = newValue / 100.0f;
}

void BandLimitedOscillator::setPolyBLEPMix(float newValue)
{
    polyBLEPMix = newValue / 100.0f;
}
void BandLimitedOscillator::setWaveShapeSaturation(float newValue)
{
    waveShapeSaturation = newValue;
}

void BandLimitedOscillator::setVolume(float newValue)
{
    volume = newValue;
}

void BandLimitedOscillator::reset(double inSampleRate)
{
    sampleRate = inSampleRate;
    modFrequency = 0.0;
    dpwDifferentiator.reset(0.0);
    noteOn = false;
}

double BandLimitedOscillator::getStartingPhaseOffset()
{
    switch (waveType)
    {
        case BLEPSaw:
        case PolyBLEPSawSquareMod:
        case UnipolarWaveShapedSaw:
        case BipolarWaveShapedSaw:
        case SumofSawSquarePWM:
            return 0.5;

        case DPWTriangle:
        case ParabolicSine:
        case WhiteNoise:
        case RandomNoise:
            return 0.0;
    }
}

void BandLimitedOscillator::startNote(double frequency)
{
    modFrequency = frequency * getPitchFreqMod(octaves, semitones, cents);
    phaseAccumulator.reset(getStartingPhaseOffset(), modFrequency / sampleRate);
    dpwPhaseAccumulator.reset(getStartingPhaseOffset(), (2 * modFrequency) / sampleRate);
    noteOn = true;
}

void BandLimitedOscillator::stopNote()
{
    noteOn = false;
    modFrequency = 0.0;
}

double BandLimitedOscillator::getNextSample()
{
    double sample = 0.0;
    
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
        
        phaseAccumulator.IncrementPhase();
        dpwPhaseAccumulator.IncrementPhase();
    }
    
    return sample * volume;
}

double BandLimitedOscillator::getNextPolyBLEPSawSquareModSample()
{
    double phase = phaseAccumulator.getPhase();
    double phaseInc = phaseAccumulator.getPhaseInc();
    
    double alternatePhase = phase + 0.5;
    if (alternatePhase > 1.0)
        alternatePhase -= 1.0;

    double sawSample =  getTrivialSawSample(phase) - polyBLEP(phase, phaseInc);
    
    double squareSample = phase > 0.5 ? 1.0 : -1.0;
    squareSample += polyBLEP(alternatePhase, phaseInc) - polyBLEP(phase, phaseInc);
    
    // Mix the square and saw waves together
    return squareSample * polyBLEPMix + (sawSample * (1.0 - polyBLEPMix));
}

double BandLimitedOscillator::getNextBLEPSawSample()
{
    const double* blepTable = nullptr;
    size_t pointsPerSide = 0;
    const size_t tableLength = 4096;
    const size_t tableCenter = tableLength / 2 - 1;
    double blep = 0.0;
    
    double phase = phaseAccumulator.getPhase();
    double phaseInc = phaseAccumulator.getPhaseInc();

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
            double index = (1.0 + t) * tableCenter;
            double frac = index - int(index);
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
            double index = t * tableCenter + (tableCenter + 1.0);
            double frac = index - int(index);
            blep = linear_interp(blepTable[(int)index], blepTable[(int)index + 1], frac);
        }
    }
    
    return getTrivialSawSample(phase) - blep;
}

double BandLimitedOscillator::getNextUnipolarWaveShapedSawSample()
{
    double phase = phaseAccumulator.getPhase();
    double phaseInc = phaseAccumulator.getPhaseInc();

    double waveShapedSawSample = tanh(waveShapeSaturation * phase) / tanh(waveShapeSaturation);
    return getTrivialSawSample(waveShapedSawSample) - polyBLEP(phase, phaseInc);
}

double BandLimitedOscillator::getNextBipolarWaveShapedSawSample()
{
    double phase = phaseAccumulator.getPhase();
    double phaseInc = phaseAccumulator.getPhaseInc();

    double trivialSawSample = getTrivialSawSample(phase);
    return tanh(waveShapeSaturation * trivialSawSample) / tanh(waveShapeSaturation) - polyBLEP(phase, phaseInc);
}

double BandLimitedOscillator::getNextSumOfSawSquarePWMSample()
{
    double phase = phaseAccumulator.getPhase();
    double phaseInc = phaseAccumulator.getPhaseInc();

    double phaseMod = phase;
    double saw1 = getTrivialSawSample(phase);
    
    if (phaseInc > 0)
        phaseMod += pulseWidth;
    else
        phaseMod -= pulseWidth;

    if (phaseInc > 0 && phaseMod >= 1.0)
        phaseMod -= 1.0;
    
    if (phaseInc < 0 && phaseMod <= 0)
        phaseMod += 1.0;
    
    double saw2 = getTrivialSawSample(phaseMod);
    double sample = 0.5 * saw1 - 0.5 * saw2;
    
    // DC Correction
    double correction = 1.0 / pulseWidth ;
    if (pulseWidth < 0.5)
        correction = 1.0 / (1.0 - pulseWidth);
    
    return sample *= correction;
}

double BandLimitedOscillator::getNextDPWTriangleSample()
{
    double phase = phaseAccumulator.getPhase();

    double squareModulator = getTrivialPulseSample(phase, 0.5);

    double sawSample = getTrivialSawSample(dpwPhaseAccumulator.getPhase());
    double invertedSquaredSaw = 1.0 - sawSample * sawSample;
    
    double squareMod = invertedSquaredSaw * squareModulator;
    double differentiatedSquareMod = dpwDifferentiator.process(squareMod);

    double c = sampleRate / (4.0 * 2.0 * modFrequency * (1 - dpwPhaseAccumulator.getPhaseInc()));
    return differentiatedSquareMod * c;
}

double BandLimitedOscillator::getNextParabolicSineSample()
{
    return (float)(parabolicSine(phaseAccumulator.getPhase() * 2.0 * pi - pi));
}

double BandLimitedOscillator::getNextWhiteNoiseSample()
{
    return unipolarToBipolar(arc4random_uniform(UINT32_MAX) / (double)UINT32_MAX);
}

double BandLimitedOscillator::getNextRandomNoiseSample()
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
    double sample = (float)(seed) / (pow(2.0, 32.0) / 16.0);
    
    // shift down to form a result from -1.0 to +1.0
    sample -= 1.0;
    
    return sample;
}
