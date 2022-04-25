#pragma once

#include "TooManyTweaks.hpp"

using namespace UnityEngine;

// clang-format off
DECLARE_CONFIG(TMTConfig,
    CONFIG_VALUE(enableCustomRumble, bool, "Enable Custom Rumble", false)
    CONFIG_VALUE(cutRumbleStrength, float, "Cut Rumble Strength", 1.0f)
    CONFIG_VALUE(chainRumbleStrength, float, "Chain Rumble Strength", 1.0f)
    CONFIG_VALUE(arcRumbleStrength, float, "Arc Rumble Strength", 1.0f)

    CONFIG_VALUE(disableDebris, bool, "Disable Debris", false)
//    CONFIG_VALUE(transparentWalls, bool, "Transparent Walls", false)
    CONFIG_VALUE(disableSliceScore, bool, "Disable Slice Score", false)
    CONFIG_VALUE(disableBeatLines, bool, "Disable Beat Lines", false)

    CONFIG_VALUE(noHealthAndSafety, bool, "Disable Health Warning", true)
    CONFIG_VALUE(noComboBreakEffect, bool, "Disable Combo Break Effect", false)

    CONFIG_INIT_FUNCTION(
            CONFIG_INIT_VALUE(enableCustomRumble)
            CONFIG_INIT_VALUE(cutRumbleStrength)
            CONFIG_INIT_VALUE(chainRumbleStrength)
            CONFIG_INIT_VALUE(arcRumbleStrength)

            CONFIG_INIT_VALUE(disableDebris)
            CONFIG_INIT_VALUE(disableSliceScore)
            CONFIG_INIT_VALUE(disableBeatLines)
//            CONFIG_INIT_VALUE(transparentWalls)

            CONFIG_INIT_VALUE(noHealthAndSafety)
            CONFIG_INIT_VALUE(noComboBreakEffect)
    )
)
// clang-format on