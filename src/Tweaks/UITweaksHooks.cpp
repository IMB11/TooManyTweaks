#include "TooManyTweaks.hpp"

#include "GlobalNamespace/DefaultScenesTransitionsFromInit.hpp"
#include "GlobalNamespace/ComboUIController.hpp"
#include "GlobalNamespace/MusicPackPromoBanner.hpp"
#include "UnityEngine/Animator.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

MAKE_HOOK_MATCH(healthStopper, &DefaultScenesTransitionsFromInit::TransitionToNextScene, void,
                DefaultScenesTransitionsFromInit *self, bool goStraightToMenu, bool goStraightToEditor, bool goToRecordingToolScene) {
    healthStopper(self, getTMTConfig().noHealthAndSafety.GetValue(), goStraightToEditor, goToRecordingToolScene);
}

MAKE_HOOK_MATCH(promoHider,
                &GlobalNamespace::MainMenuViewController::DidActivate, void,
                GlobalNamespace::MainMenuViewController *self,
                bool firstActivation, bool addedToHierarchy,
                bool screenSystemEnabling) {
    promoHider(self, firstActivation, addedToHierarchy,
                                       screenSystemEnabling);

    self->musicPackPromoBanner->get_gameObject()->SetActive(!getTMTConfig().noPromo.GetValue());
}

MAKE_HOOK_MATCH(comboAnimation, &ComboUIController::Start, void, ComboUIController *self) {
    comboAnimation(self);

    if (getTMTConfig().noComboBreakEffect.GetValue()) self->animator->set_speed(69420.0f); // Yeet!
}

void InstallUIHooks(Logger &logger) {
    INSTALL_HOOK(logger, healthStopper)
    INSTALL_HOOK(logger, comboAnimation)
    INSTALL_HOOK(logger, promoHider)
}

RegisterMTHooks(InstallUIHooks)