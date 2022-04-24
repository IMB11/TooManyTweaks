#pragma once

#include "TooManyTweaks.hpp"

#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"
#include "GlobalNamespace/HapticFeedbackController.hpp"

#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SaberTypeExtensions.hpp"

using namespace Libraries::HM::HMLib::VR;

static const float STRENGTH_NORMAL = 1.0f;
static const float DURATION_NORMAL = 0.14f;

static const float STRENGTH_WEAK = 0.75f;
static const float DURATION_WEAK = 0.07f;

static const float ARC_STRENGTH_NORMAL = 0.75f;
static const float ARC_DURATION_NORMAL = 0.01f;

static HapticPresetSO* normalPreset;
static HapticPresetSO* weakPreset;
static HapticPresetSO* arcPreset;

