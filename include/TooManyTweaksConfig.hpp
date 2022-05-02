#pragma once

#include "TooManyTweaks.hpp"

using namespace UnityEngine;

#define CIV(args...) CONFIG_INIT_VALUE(args)

// clang-format off
DECLARE_CONFIG(TMTConfig,

                       CONFIG_VALUE(enableCustomRumble, bool, "Enable Custom Rumble", false)
                       CONFIG_VALUE(cutRumbleStrength, float, "Cut Rumble Strength", 1.0f)
                       CONFIG_VALUE(chainRumbleStrength, float, "Chain Rumble Strength", 1.0f)
                       CONFIG_VALUE(arcRumbleStrength, float, "Arc Rumble Strength", 1.0f)
                       CONFIG_VALUE(ignoreSaberSpeed, bool, "Ignore Saber Speed", false)
                       CONFIG_VALUE(ignoreBadCuts, bool, "Ignore Bad Cuts", false)
                       CONFIG_VALUE(staticSoundPos, bool, "Static Sound Position", false)
                       CONFIG_VALUE(enableSpatialization, bool, "Enable Sound Spatialization", true)
                       CONFIG_VALUE(randomPitchMin, float, "Min Random Pitch", 0.9f)
                       CONFIG_VALUE(randomPitchMax, float, "Max Random Pitch", 1.2f)
                       CONFIG_VALUE(enableChainLinkHitSounds, bool, "Enable Chain Link Hitsounds", false)

                       CONFIG_VALUE(chainLinkVolMultiplier, float, "Chain Link Volume Multiplier", 1.0f)

                       CONFIG_VALUE(disableDebris, bool, "Disable Debris", false)
                       CONFIG_VALUE(disableSliceScore, bool, "Disable Slice Score", false)
                       CONFIG_VALUE(disableBeatLines, bool, "Disable Beat Lines", false)
                       CONFIG_VALUE(noHealthAndSafety, bool, "Disable Health Warning", true)
                       CONFIG_VALUE(noComboBreakEffect, bool, "Disable Combo Break Effect", false)


                       CONFIG_INIT_FUNCTION(
                               CIV(enableCustomRumble)
                               CIV(cutRumbleStrength)
                               CIV(chainRumbleStrength)
                               CIV(arcRumbleStrength)
                               CIV(ignoreSaberSpeed)
                               CIV(ignoreBadCuts)
                               CIV(staticSoundPos)
                               CIV(enableSpatialization)
                               CIV(randomPitchMin)
                               CIV(randomPitchMax)
                               CIV(enableChainLinkHitSounds)
                               CIV(chainLinkVolMultiplier)
                               CIV(disableDebris)
                               CIV(disableSliceScore)
                               CIV(disableBeatLines)
                               CIV(noHealthAndSafety)
                               CIV(noComboBreakEffect)

                       )
)
// clang-format on