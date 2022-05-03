#include "TooManyTweaks.hpp"

#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/RandomObjectPicker_1.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/AnimationCurve.hpp"
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/AudioSettings.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Time.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/MemoryPoolContainer_1.hpp"
#include "GlobalNamespace/LazyCopyHashSet_1.hpp"
#include "GlobalNamespace/INoteCutSoundEffectDidFinishEvent.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/NoteCutSoundEffectManager_InitData.hpp"

#include "UnityEngine/Random.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

//void (*set_spatialize)(AudioSource*, bool val) = TooManyTweaks::resolve_icall<void, AudioSource*, bool>("UnityEngine.AudioSource::set_spatialize");
//
//bool (*get_spatialize)(AudioSource*) = TooManyTweaks::resolve_icall<bool, AudioSource*>("UnityEngine.AudioSource::get_spatialize");

MAKE_HOOK_MATCH(ChainHSElmPatch, &NoteCutSoundEffectManager::IsSupportedNote, bool, NoteCutSoundEffectManager *self,
                NoteData *data) {
    auto orig = ChainHSElmPatch(self, data);
    if (data->get_gameplayType() != NoteData::GameplayType::BurstSliderElement ||
        data->get_colorType() == ColorType::_get_None())
        return getTMTConfig().enableChainLinkHitSounds.GetValue();

    return orig;
}

MAKE_HOOK_MATCH(NCSE_Init, &NoteCutSoundEffect::Init, void, NoteCutSoundEffect *self, UnityEngine::AudioClip *audioClip,
                GlobalNamespace::NoteController *noteController, double noteDSPTime, float aheadTime,
                float missedTimeOffset, float timeToPrevNote, float timeToNextNote, GlobalNamespace::Saber *saber,
                bool handleWrongSaberTypeAsGood, float volumeMultiplier, bool ignoreSaberSpeed, bool ignoreBadCuts) {
    if (noteController->dyn__noteData()->get_gameplayType() == NoteData::GameplayType::_get_BurstSliderElement()) {
        volumeMultiplier *= getTMTConfig().chainLinkVolMultiplier.GetValue();
    }

    const float hitsoundAlignOffset = 0.185f;
    auto sfxLatency = getTMTConfig().enableSpatialization.GetValue() ? aheadTime - hitsoundAlignOffset : 0.0f;

    auto randomPitch = Random::Range(getTMTConfig().randomPitchMin.GetValue(),
                                     getTMTConfig().randomPitchMax.GetValue());

    aheadTime = (hitsoundAlignOffset / randomPitch + sfxLatency) * randomPitch;

//    set_spatialize(self->dyn__audioSource(), getTMTConfig().enableSpatialization.GetValue());

    self->dyn__pitch() = randomPitch;
    self->dyn__ignoreSaberSpeed() = getTMTConfig().ignoreSaberSpeed.GetValue();
    self->dyn__ignoreBadCuts() = getTMTConfig().ignoreBadCuts.GetValue();
    self->dyn__beforeCutVolume() = 0.0f;
    self->dyn__volumeMultiplier() = volumeMultiplier;
    self->set_enabled(true);
    self->dyn__audioSource()->set_clip(audioClip);
    self->dyn__noteMissedTimeOffset() = missedTimeOffset;
    self->dyn__aheadTime() = aheadTime;
    self->dyn__timeToNextNote() = timeToNextNote;
    self->dyn__timeToPrevNote() = timeToPrevNote;
    self->dyn__saber() = saber;
    self->dyn__noteController() = noteController;
    self->dyn__handleWrongSaberTypeAsGood() = handleWrongSaberTypeAsGood;
    self->dyn__noteWasCut() = false;
    if (self->dyn__ignoreSaberSpeed()) {
        self->dyn__beforeCutVolume() = self->dyn__goodCutVolume();
        self->dyn__audioSource()->set_volume(self->dyn__goodCutVolume());
    } else {
        self->dyn__beforeCutVolume() = 0.0f;
        self->dyn__audioSource()->set_volume(self->dyn__speedToVolumeCurve()->Evaluate(saber->get_bladeSpeed()));
    }
    self->dyn__audioSource()->set_pitch(self->dyn__pitch());
    self->dyn__audioSource()->set_priority(128);
    self->get_transform()->set_position(saber->get_saberBladeTopPos());
    self->ComputeDSPTimes(noteDSPTime, self->dyn__aheadTime(), timeToPrevNote, timeToNextNote);
    self->dyn__audioSource()->PlayScheduled(self->dyn__startDSPTime());

    self->get_transform()->set_position({0, 0, 0});

    if (!getTMTConfig().staticSoundPos.GetValue()) {
        self->get_transform()->set_position(self->dyn__saber()->get_saberBladeTopPos());
    }
}

MAKE_HOOK_MATCH(NCSEM_HandleHit, &NoteCutSoundEffectManager::HandleNoteWasSpawned, void,
                NoteCutSoundEffectManager *self, NoteController *noteController) {
    auto noteData = noteController->get_noteData();
    if (!self->IsSupportedNote(noteData)) {
        return;
    }
    float cutSfxVolumeMultiplier = noteData->get_cutSfxVolumeMultiplier();
    if (cutSfxVolumeMultiplier <= 0.0f) {
        return;
    }

    if ((noteData->get_colorType() == ColorType::ColorA &&
         std::abs(noteData->get_time() - self->dyn__prevNoteATime()) < 0.001f) ||
        (noteData->get_colorType() == ColorType::ColorB &&
         std::abs(noteData->get_time() - self->dyn__prevNoteBTime()) < 0.001f)) {
        return;
    }

    float timeScale = self->dyn__audioTimeSyncController()->dyn__timeScale();
    auto noteCutSoundEffect = self->dyn__noteCutSoundEffectPoolContainer()->Spawn();
    noteCutSoundEffect->get_transform()->SetPositionAndRotation(self->get_transform()->get_localPosition(),
                                                                Quaternion::get_identity());
    noteCutSoundEffect->dyn__didFinishEvent()->Add(reinterpret_cast<INoteCutSoundEffectDidFinishEvent *>(self));
    Saber *saber = nullptr;
    if (noteData->get_colorType() == ColorType::ColorA) {
        self->dyn__prevNoteATime() = noteData->get_time();
        saber = self->dyn__saberManager()->dyn__leftSaber();
        self->dyn__prevNoteASoundEffect() = noteCutSoundEffect;
    } else if (noteData->get_colorType() == ColorType::ColorB) {
        self->dyn__prevNoteBTime() = noteData->get_time();
        saber = self->dyn__saberManager()->dyn__rightSaber();
        self->dyn__prevNoteBSoundEffect() = noteCutSoundEffect;
    }
    bool flag2 = noteData->get_timeToPrevColorNote() < self->dyn__beatAlignOffset();
    AudioClip *audioClip = flag2 ? self->dyn__randomShortCutSoundPicker()->PickRandomObject()
                                 : self->dyn__randomLongCutSoundPicker()->PickRandomObject();
    float num = 0.9f;
    noteCutSoundEffect->Init(audioClip, noteController, (double) (noteData->get_time() / timeScale) +
                                                        self->dyn__audioTimeSyncController()->get_dspTimeOffset() +
                                                        (double) self->dyn__audioTimeSyncController()->get_songTimeOffset(),
                             self->dyn__beatAlignOffset(), 0.15f, noteData->get_timeToPrevColorNote() / timeScale,
                             noteData->get_timeToNextColorNote() / timeScale, saber,
                             self->get_handleWrongSaberTypeAsGood(), num * cutSfxVolumeMultiplier,
                             self->dyn__useTestAudioClips(), self->dyn__initData()->dyn_ignoreBadCuts());
    ListW<NoteCutSoundEffect *> activeItems = self->dyn__noteCutSoundEffectPoolContainer()->dyn__activeItems()->get_items();
    NoteCutSoundEffect *noteCutSoundEffect2 = nullptr;
    float num2 = std::numeric_limits<float>::max();

    for (NoteCutSoundEffect *noteCutSoundEffect3: activeItems) {
        if (noteCutSoundEffect3->get_time() < num2) {
            num2 = noteCutSoundEffect3->get_time();
            noteCutSoundEffect2 = noteCutSoundEffect3;
        }
    }
    if (activeItems.size() > 64 && noteCutSoundEffect2 != nullptr) {
        noteCutSoundEffect2->StopPlayingAndFinish();
    }

}

MAKE_HOOK_MATCH(NCSE_NoteWasCut, &NoteCutSoundEffect::NoteWasCut, void, NoteCutSoundEffect *self,
                NoteController *noteController, ByRef<NoteCutInfo> noteCutInfo) {
    if (self->dyn__noteController() != noteController) {
        return;
    }

    self->dyn__noteWasCut() = true;

    if (!self->dyn__ignoreBadCuts() && ((!self->dyn__handleWrongSaberTypeAsGood() && !noteCutInfo->get_allIsOK()) ||
                                        self->dyn__handleWrongSaberTypeAsGood() &&
                                        (!noteCutInfo->get_allExceptSaberTypeIsOK() || noteCutInfo->saberTypeOK))) {
        self->dyn__audioSource()->set_priority(16);
        auto clip = self->dyn__badCutRandomSoundPicker()->PickRandomObject();
        self->dyn__audioSource()->set_clip(clip);
        self->dyn__audioSource()->set_time(0.0f);
        self->dyn__audioSource()->Play();
        self->dyn__goodCut() = false;
        self->dyn__audioSource()->set_volume(self->dyn__badCutVolume());
        self->dyn__endDSPtime() = AudioSettings::get_dspTime() +
                                  (self->dyn__audioSource()->get_clip()->get_length() / self->dyn__pitch()) +
                                  0.10000000149011612;
    } else {
        self->dyn__audioSource()->set_priority(24);
        self->dyn__goodCut() = true;
        self->dyn__audioSource()->set_volume(self->dyn__goodCutVolume());
    }

    if (!self->dyn__goodCut()) {
//        set_spatialize(self->dyn__audioSource(), true);
        self->get_transform()->set_position(noteCutInfo->dyn_cutPoint());
        return;
    }

    if (getTMTConfig().enableSpatialization.GetValue()) {
        self->get_transform()->set_position(noteCutInfo->dyn_cutPoint());
    }
}

MAKE_HOOK_MATCH(NCSE_LateUpdate, &NoteCutSoundEffect::LateUpdate, void, NoteCutSoundEffect *self) {
    double dspTime = AudioSettings::get_dspTime();
    if (dspTime - self->dyn__endDSPtime() > 0.0) {
        self->StopPlayingAndFinish();
        return;
    }
    if (!self->dyn__noteWasCut()) {
        if (dspTime > self->dyn__startDSPTime() + (double) self->dyn__aheadTime() - 0.05000000074505806) {
            self->dyn__audioSource()->set_priority(32);
        }
        float num = self->dyn__goodCutVolume();
        if (!self->dyn__ignoreSaberSpeed()) {
            num *= self->dyn__speedToVolumeCurve()->Evaluate(self->dyn__saber()->get_bladeSpeed()) * (1.0f -
                                                                                                      Mathf::Clamp01(
                                                                                                              (self->dyn__audioSource()->get_time() /
                                                                                                               self->dyn__pitch() -
                                                                                                               self->dyn__aheadTime()) /
                                                                                                              self->dyn__noteMissedTimeOffset()));
        }
        if (num < self->dyn__beforeCutVolume()) {
            self->dyn__beforeCutVolume() = std::lerp(self->dyn__beforeCutVolume(), num, Time::get_deltaTime() * 4.0f);
        } else {
            self->dyn__beforeCutVolume() = num;
        }
        self->dyn__audioSource()->set_volume(self->dyn__beforeCutVolume() * self->dyn__volumeMultiplier());
        return;
    }
    if (self->dyn__goodCut()) {
        self->dyn__audioSource()->set_volume(self->dyn__goodCutVolume() * self->dyn__volumeMultiplier());
    }

    if (!self->dyn__noteWasCut() && !getTMTConfig().staticSoundPos.GetValue()) {
        self->get_transform()->set_position(self->dyn__saber()->get_saberBladeTopPos());
    }
}

void InstallHitsoundTweaksHooks(Logger &logger) {
    INSTALL_HOOK(logger, ChainHSElmPatch)
    INSTALL_HOOK(logger, NCSE_Init)
    INSTALL_HOOK(logger, NCSE_NoteWasCut)
    INSTALL_HOOK(logger, NCSE_LateUpdate)
    INSTALL_HOOK(logger, NCSEM_HandleHit)
}

RegisterMTHooks(InstallHitsoundTweaksHooks)