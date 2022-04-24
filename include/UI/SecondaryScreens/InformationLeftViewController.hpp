#pragma once

#include "TooManyTweaks.hpp"

// clang-format off
DECLARE_CLASS_CODEGEN(TooManyTweaks, InformationLeftViewController, HMUI::ViewController,
                      DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
)
// clang-format on