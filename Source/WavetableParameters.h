/*
  ==============================================================================

    WavetableParameters.h
    Created: 17 Jun 2019 8:40:20pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

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

const float wtbPositionMinValue = 0.0f;
const float wtbPositionMaxValue = 0.99999f;
const float wtbPositionInitialValue = 0.0f;

const bool wtbInterpolateInitialValue = false;

const int wtbSemitonesMinValue = -36;
const int wtbSemitonesMaxValue = 36;
const int wtbSemitonesInitialValue = 0;

const float wtbCentsMinValue = -1.0f;
const float wtbCentsMaxValue = 1.0f;
const float wtbCentsInitialValue = 0.0f;

const float wtbVolumeMinValue = 0.0f;
const float wtbVolumeMaxValue = 1.0f;
const float wtbVolumeInitialValue = 1.0f;
