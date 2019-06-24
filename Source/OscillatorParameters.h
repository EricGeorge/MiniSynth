/*
  ==============================================================================

    OscillatorParameters.h
    Created: 22 May 2019 11:50:26am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cstring>

// Band Limited Oscillators
enum OscillatorParameters
{
    kOscParam_WaveType = 0,
    kOscParam_Octave,
    kOscParam_Semitone,
    kOscParam_Cents,
    kOscParam_PulseWidth,
    kOscParam_PolyBLEPMix,
    kOscParam_WaveShapeSaturation,
    kOscParam_Volume
};

static String oscillatorParamIDPrefix = "Oscillator_";
static String oscillator_ParamIDs[] =
{
    "Oscillator_Wavetype",
    "Oscillator_Octave",
    "Oscillator_Semitone",
    "Oscillator_Cents",
    "Oscillator_PulseWidth",
    "Oscillator_PolyBLEPMix",
    "Oscillator_WaveShapeSaturation",
    "Oscillator_Volume"
};

// Low Frequency Oscillators

enum LfoParameters
{
    kLfoParam_WaveType = 0,
    kLfoParam_RunState,
    kLfoParam_PulseWidth,
    kLfoParam_PhaseOffset,
    kLfoParam_Amount,
    kLfoParam_PolarityOffset,
    kLfoParam_Rate,
    kLfoParam_Sync,
    kLfoParam_FadeInTime,
    kLfoParam_Delay
};

static String lfoParamIDPrefix = "LFO_";
static String lfo_ParamIDs[] =
{
    "LFO_Wavetype",
    "LFO_RunState",
    "LFO_PulseWidth",
    "LFO_PhaseOffset",
    "LFO_Amount",
    "LFO_PolarityOffset",
    "LFO_Rate",
    "LFO_Sync",
    "LFO_FadeInTime",
    "LFO_Delay"
};
