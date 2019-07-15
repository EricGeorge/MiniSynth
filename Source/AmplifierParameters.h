/*
  ==============================================================================

    AmplifierParameters.h
    Created: 6 Jul 2019 7:16:29pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum AmplifierParameters
{
    kAmpParam_Gain = 0,
    kAmpParam_Pan
};

static String amplifierParamIDPrefix = "Amplifier_";
static String amplifier_ParamIDs[] =
{
    "Amplifier_Gain",
    "Amplifier_Pan"
};

const float ampGainMinValue = 0.0f;
const float ampGainMaxValue = 1.0f;
const float ampGainInitialValue = 1.0f;

const float ampPanMinValue = -1.0f;
const float ampPanMaxValue = 1.0f;
const float ampPanInitialValue = 0.0f;
