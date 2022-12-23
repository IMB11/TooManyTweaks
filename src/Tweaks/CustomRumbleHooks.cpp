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

    weakPreset->duration = std::min(0.2f, DURATION_WEAK * getTMTConfig().chainRumbleStrength.GetValue());
    weakPreset->strength = STRENGTH_WEAK * std::min(1.0f, getTMTConfig().chainRumbleStrength.GetValue() * 1.2f);

    normalPreset->duration = std::min(0.2f, DURATION_NORMAL * getTMTConfig().cutRumbleStrength.GetValue());
    normalPreset->strength = STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().cutRumbleStrength.GetValue() * 1.2f);

    if(type == NoteCutHapticEffect::Type::_get_ShortWeak()) {
        if(weakPreset->duration == 0 || weakPreset->strength == 0) return;

        self->hapticFeedbackController->PlayHapticFeedback(SaberTypeExtensions::Node(saberType), weakPreset);
    } else {
        if(normalPreset->duration == 0 || normalPreset->strength == 0) return;

        self->hapticFeedbackController->PlayHapticFeedback(SaberTypeExtensions::Node(saberType), normalPreset);
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

    arcPreset->continuous = true;
    arcPreset->duration = std::min(0.2f, ARC_DURATION_NORMAL * getTMTConfig().arcRumbleStrength.GetValue());
    arcPreset->strength = ARC_STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().arcRumbleStrength.GetValue() * 1.2f);

    self->hapticPreset = arcPreset;

    ArcRumbleHook(self);
}

void InstallRumbleHooks(Logger& logger) {
    INSTALL_HOOK(logger, CutRumbleHook);
    INSTALL_HOOK(logger, ArcRumbleHook);
}

RegisterMTHooks(InstallRumbleHooks)