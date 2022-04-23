#pragma once

#include "TooManyTweaks.hpp"

using namespace UnityEngine;

// clang-format off
DECLARE_CONFIG(TMTConfig,
    CONFIG_VALUE(enableCustomRumble, bool, "Enable Custom Rumble", false)
    CONFIG_VALUE(cutRumbleStrength, float, "Cut Rumble Strength", 1.0f)
    CONFIG_VALUE(chainRumbleStrength, float, "Chain Rumble Strength", 1.0f)
    CONFIG_VALUE(arcRumbleStrength, float, "Arc Rumble Strength", 1.0f)

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(enableCustomRumble)
        CONFIG_INIT_VALUE(cutRumbleStrength)
        CONFIG_INIT_VALUE(chainRumbleStrength)
        CONFIG_INIT_VALUE(arcRumbleStrength)
    )
)
// clang-format on