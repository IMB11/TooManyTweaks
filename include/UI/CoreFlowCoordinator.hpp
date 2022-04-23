#pragma once

#include "TooManyTweaks.hpp"

#include "HMUI/FlowCoordinator.hpp"
#include "UI/CenterViewController.hpp"

// clang-format off
DECLARE_CLASS_CODEGEN(TooManyTweaks, CoreFlowCoordinator, HMUI::FlowCoordinator,
            DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
            DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
            DECLARE_INSTANCE_FIELD(HMUI::ViewController*, centerView);
)
// clang-format on