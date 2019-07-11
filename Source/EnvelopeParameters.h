/*
  ==============================================================================

    EnvelopeParamters.h
    Created: 6 Jul 2019 6:54:22pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum EnvelopeParameters
{
    kEnvParam_Attack = 0,
    kEnvParam_Decay,
    kEnvParam_Sustain,
    kEnvParam_Release
};

static String envelopeParamIDPrefix = "Envelope_";
static String envelope_ParamIDs[] =
{
    "Envelope_Attack",
    "Envelope_Decay",
    "Envelope_Sustain",
    "Envelope_Release"
};

// milliseconds
const float envAttackMinValue = 1.0f;
const float envAttackMaxValue = 20000.0f;
const float envAttackInitialValue = 100.0f;

const float envDecayMinValue = 1.0f;
const float envDecayMaxValue = 20000.0f;
const float envDecayInitialValue = 100.0f;

const float envSustainMinValue = 0.0f;
const float envSustainMaxValue = 1.0f;
const float envSustainInitialValue = 0.7f;

const float envReleaseMinValue = 1.0f;
const float envReleaseMaxValue = 20000.0f;
const float envReleaseInitialValue = 100.0f;
