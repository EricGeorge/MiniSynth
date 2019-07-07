/*
  ==============================================================================

    AmplifierParameters.h
    Created: 6 Jul 2019 7:16:29pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cstring>

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
