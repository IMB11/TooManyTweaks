#include "Hooks/CustomRumbleHooks.hpp"

#include "GlobalNamespace/NoteCutHapticEffect.hpp"
#include "GlobalNamespace/NoteCutHapticEffect_Type.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SaberTypeExtensions.hpp"
#include "GlobalNamespace/SliderHapticFeedbackInteractionEffect.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

MAKE_HOOK_MATCH(CutRumbleHook, &NoteCutHapticEffect::HitNote, void, NoteCutHapticEffect* self, SaberType saberType, NoteCutHapticEffect::Type type) {
    if(!getTMTConfig().enableCustomRumble.GetValue()) {
        CutRumbleHook(self, saberType, type);
        return;
    }

    if(normalPreset == nullptr) {
        normalPreset = ScriptableObject::CreateInstance<HapticPresetSO *>();
    }

    if(weakPreset == nullptr) {
        weakPreset = ScriptableObject::CreateInstance<HapticPresetSO *>();
    }

    weakPreset->dyn__duration() = std::min(0.2f, DURATION_WEAK * getTMTConfig().chainRumbleStrength.GetValue());
    weakPreset->dyn__strength() = STRENGTH_WEAK * std::min(1.0f, getTMTConfig().chainRumbleStrength.GetValue() * 1.2f);

    normalPreset->dyn__duration() = std::min(0.2f, DURATION_NORMAL * getTMTConfig().cutRumbleStrength.GetValue());
    normalPreset->dyn__strength() = STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().cutRumbleStrength.GetValue() * 1.2f);

    if(type == NoteCutHapticEffect::Type::_get_ShortWeak()) {
        if(weakPreset->dyn__duration() == 0 || weakPreset->dyn__strength() == 0) return;

        self->dyn__hapticFeedbackController()->PlayHapticFeedback(SaberTypeExtensions::Node(saberType), weakPreset);
    } else {
        if(normalPreset->dyn__duration() == 0 || normalPreset->dyn__strength() == 0) return;

        self->dyn__hapticFeedbackController()->PlayHapticFeedback(SaberTypeExtensions::Node(saberType), normalPreset);
    }
}



MAKE_HOOK_MATCH(ArcRumbleHook, &SliderHapticFeedbackInteractionEffect::Vibrate, void, SliderHapticFeedbackInteractionEffect* self) {
    if(!getTMTConfig().enableCustomRumble.GetValue()) {
        ArcRumbleHook(self);
        return;
    }

    if(arcPreset == nullptr) {
        arcPreset = ScriptableObject::CreateInstance<HapticPresetSO*>();
    }

    arcPreset->dyn__continuous() = true;
    arcPreset->dyn__duration() = std::min(0.2f, ARC_DURATION_NORMAL * getTMTConfig().arcRumbleStrength.GetValue());
    arcPreset->dyn__strength() = ARC_STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().arcRumbleStrength.GetValue() * 1.2f);

    self->dyn__hapticPreset() = arcPreset;

    ArcRumbleHook(self);
}

void InstallRumbleHooks(Logger& logger) {
    INSTALL_HOOK(logger, CutRumbleHook);
    INSTALL_HOOK(logger, ArcRumbleHook);
}

RegisterMTHooks(InstallRumbleHooks)