#pragma once

#include "TooManyTweaks.hpp"
#include "CoreFlowCoordinator.hpp"

// clang-format off
DECLARE_CLASS_CODEGEN(TooManyTweaks, CenterViewController, HMUI::ViewController,
                      DECLARE_OVERRIDE_METHOD(void, DidActivate,
                                              il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate",
                                                                             3),
                                              bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                              DECLARE_INSTANCE_METHOD(void, SwitchGameplayTab, int idx);

                              DECLARE_INSTANCE_FIELD(HMUI::FlowCoordinator*, parentFlowCoordinator);

                              DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, controllerTab);
                              DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, gameplayTab);
                              DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, uiTab);
//    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, miscTab);
        public:
            std::vector<std::function<void(bool)>> onControllerTabRumbleStateChange;
            std::vector<std::function<void(bool)>> onSoundTabStateChange;
            int selectedTab;
)
// clang-format on