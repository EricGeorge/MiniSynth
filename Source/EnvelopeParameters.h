/*
  ==============================================================================

    EnvelopeParamters.h
    Created: 6 Jul 2019 6:54:22pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cstring>

enum EnvelopeParameters
{
    kEnvParam_Attack = 0,
    kEnvParam_Decay,
    kEnvParam_Sustain,
    kEnvParam_Release,
};

static String envelopeParamIDPrefix = "Envelope_";
static String envelope_ParamIDs[] =
{
    "Envelope_Attack",
    "Envelope_Decay",
    "Envelope_Sustain",
    "Envelope_Release"
};
