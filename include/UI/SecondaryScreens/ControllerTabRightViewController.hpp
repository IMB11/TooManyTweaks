#pragma once

#include "TooManyTweaks.hpp"
#include "UI/CoreFlowCoordinator.hpp"
#include "Hooks/CustomRumbleHooks.hpp"

// clang-format off
DECLARE_CLASS_CODEGEN(TooManyTweaks, ControllerTabRightViewController, HMUI::ViewController,
                      DECLARE_OVERRIDE_METHOD(void, DidActivate,
                                              il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate",
                                                                             3),
                                              bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                              DECLARE_INSTANCE_FIELD(TooManyTweaks::CenterViewController*, centerView);
                              public:
                              void testRumble(Libraries::HM::HMLib::VR::HapticPresetSO* preset, GlobalNamespace::SaberType type);
)
// clang-format on