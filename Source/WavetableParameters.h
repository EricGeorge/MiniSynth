/*
  ==============================================================================

    WavetableParameters.h
    Created: 17 Jun 2019 8:40:20pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cstring>

enum WavetableParameters
{
    kWtbParam_Position = 0,
    kWtbParam_Interpolate,
    kWtbParam_Semitones,
    kWtbParam_Cents,
    kWtbParam_Volume
};

static String wavetableParamIDPrefix = "Wavetable_";
static String wavetable_ParamIDs[] =
{
    "Wavetable_Position",
    "Wavetable_Interpolate",
    "Wavetable_Semitones",
    "Wavetable_Cents",
    "Wavetable_Volume"
};

