#include "TooManyTweaks.hpp"

#include "GlobalNamespace/DefaultScenesTransitionsFromInit.hpp"
#include "GlobalNamespace/ComboUIController.hpp"
#include "UnityEngine/Animator.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

MAKE_HOOK_MATCH(healthStopper, &DefaultScenesTransitionsFromInit::TransitionToNextScene, void, DefaultScenesTransitionsFromInit* self, bool goStraightToMenu, bool goStraightToEditor) {
    healthStopper(self, getTMTConfig().noHealthAndSafety.GetValue(), goStraightToEditor);
}

MAKE_HOOK_MATCH(comboAnimation, &ComboUIController::Start, void, ComboUIController* self) {
    comboAnimation(self);

    if(getTMTConfig().noComboBreakEffect.GetValue()) self->dyn__animator()->set_speed(69420.0f); // Yeet!
}

void InstallUIHooks(Logger& logger) {
    INSTALL_HOOK(logger, healthStopper);
    INSTALL_HOOK(logger, comboAnimation)
}

RegisterMTHooks(InstallUIHooks)