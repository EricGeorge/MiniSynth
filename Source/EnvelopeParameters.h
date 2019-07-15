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
    kEnvParam_Release,
    kEnvParam_AttackCurve,
    kEnvParam_DecayCurve,
    kEnvParam_ReleaseCurve
};

static String envelopeParamIDPrefix = "Envelope_";
static String envelope_ParamIDs[] =
{
    "Envelope_Attack",
    "Envelope_Decay",
    "Envelope_Sustain",
    "Envelope_Release",
    "Envelope_AttackCurve",
    "Envelope_DecayCurve",
    "Envelope_ReleaseCurve"
};

// milliseconds
const float envAttackMinValue = 1.0f;
const float envAttackMaxValue = 6000.0f;
const float envAttackInitialValue = 600.0f;
const float envAttackAnchorPoint = 0.5f;
const float envAttackAnchorValue = 1000.0f;

const float envAttackCurveMinValue = 0.00001f;
const float envAttackCurveMaxValue = 20.0f;
const float envAttackCurveInitialValue = 0.3f;

const float envDecayMinValue = 1.0f;
const float envDecayMaxValue = 6000.0f;
const float envDecayInitialValue = 600.0f;
const float envDecayAnchorPoint = 0.5f;
const float envDecayAnchorValue = 1000.0f;

const float envDecayCurveMinValue = 0.00001f;
const float envDecayCurveMaxValue = 20.0f;
const float envDecayCurveInitialValue = 0.001f;

const float envSustainMinValue = 0.0f;
const float envSustainMaxValue = 1.0f;
const float envSustainInitialValue = 0.7f;

const float envReleaseMinValue = 1.0f;
const float envReleaseMaxValue = 6000.0f;
const float envReleaseInitialValue = 1000.0f;
const float envReleaseAnchorPoint = 0.5f;
const float envReleaseAnchorValue = 1000.0f;

const float envReleaseCurveMinValue = 0.00001f;
const float envReleaseCurveMaxValue = 20.0f;
const float envReleaseCurveInitialValue = 0.001f;
