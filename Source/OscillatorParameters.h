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

static String oscillator1ParamIDPrefix = "Oscillator1_";
static String oscillator1_ParamIDs[] =
{
    "Oscillator1_Wavetype",
    "Oscillator1_Octave",
    "Oscillator1_Semitone",
    "Oscillator1_Cents",
    "Oscillator1_PulseWidth",
    "Oscillator1_PolyBLEPMix",
    "Oscillator1_WaveShapeSaturation",
    "Oscillator1_Volume"
};

static String oscillator2ParamIDPrefix = "Oscillator2_";
static String oscillator2_ParamIDs[] =
{
    "Oscillator2_Wavetype",
    "Oscillator2_Octave",
    "Oscillator2_Semitone",
    "Oscillator2_Cents",
    "Oscillator2_PulseWidth",
    "Oscillator2_PolyBLEPMix",
    "Oscillator2_WaveShapeSaturation",
    "Oscillator2_Volume"
};

static bool oscillator1_ParamEnables[] =
{
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true
};

static bool oscillator2_ParamEnables[] =
{
    false,
    true,
    true,
    true,
    false,
    false,
    false,
    true
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

static String lfo1ParamIDPrefix = "LFO1_";
static String lfo1_ParamIDs[] =
{
    "LFO1_Wavetype",
    "LFO1_RunState",
    "LFO1_PulseWidth",
    "LFO1_PhaseOffset",
    "LFO1_Amount",
    "LFO1_PolarityOffset",
    "LFO1_Rate",
    "LFO1_Sync",
    "LFO1_FadeInTime",
    "LFO1_Delay"
};
