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
    if (data->get_gameplayType() == NoteData::GameplayType::BurstSliderElement)
        return getTMTConfig().enableChainLinkHitSounds.GetValue();

    return orig;
}

MAKE_HOOK_MATCH(NCSE_Init, &NoteCutSoundEffect::Init, void, NoteCutSoundEffect *self, UnityEngine::AudioClip *audioClip,
                GlobalNamespace::NoteController *noteController, double noteDSPTime, float aheadTime,
                float missedTimeOffset, float timeToPrevNote, float timeToNextNote, GlobalNamespace::Saber *saber,
                bool handleWrongSaberTypeAsGood, float volumeMultiplier, bool ignoreSaberSpeed, bool ignoreBadCuts) {


    if (noteController->noteData->get_gameplayType() == NoteData::GameplayType::_get_BurstSliderElement()) {

        volumeMultiplier *= getTMTConfig().chainLinkVolMultiplier.GetValue();

    }


    const float hitsoundAlignOffset = 0.185f;

    auto sfxLatency = getTMTConfig().enableSpatialization.GetValue() ? aheadTime - hitsoundAlignOffset : 0.0f;


    auto randomPitch = Random::Range(getTMTConfig().randomPitchMin.GetValue(),
                                     getTMTConfig().randomPitchMax.GetValue());


    aheadTime = (hitsoundAlignOffset / randomPitch + sfxLatency) * randomPitch;


    self->pitch = randomPitch;

    self->ignoreSaberSpeed = getTMTConfig().ignoreSaberSpeed.GetValue();

    self->ignoreBadCuts = getTMTConfig().ignoreBadCuts.GetValue();

    self->beforeCutVolume = 0.0f;

    self->volumeMultiplier = volumeMultiplier;

    self->set_enabled(true);

    self->audioSource->set_clip(audioClip);

    self->noteMissedTimeOffset = missedTimeOffset;

    self->aheadTime = aheadTime;

    self->timeToNextNote = timeToNextNote;

    self->timeToPrevNote = timeToPrevNote;

    self->saber = saber;

    self->noteController = noteController;

    self->handleWrongSaberTypeAsGood = handleWrongSaberTypeAsGood;

    self->noteWasCut = false;

    if (self->ignoreSaberSpeed) {

        self->beforeCutVolume = self->goodCutVolume;

        self->audioSource->set_volume(self->goodCutVolume);

    } else {

        self->beforeCutVolume = 0.0f;

        self->audioSource->set_volume(
                self->speedToVolumeCurve->Evaluate(self->saber->get_bladeSpeed()));

    }

    self->audioSource->set_pitch(self->pitch);

    self->audioSource->set_priority(128);

    self->get_transform()->set_position(self->saber->get_saberBladeTopPos());

    self->ComputeDSPTimes(noteDSPTime, self->aheadTime, timeToPrevNote, timeToNextNote);

    self->audioSource->PlayScheduled(self->startDSPTime);


    self->get_transform()->set_position({0, 0, 0});


    if (!getTMTConfig().staticSoundPos.GetValue()) {

        self->get_transform()->set_position(self->saber->get_saberBladeTopPos());

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
         std::abs(noteData->get_time() - self->prevNoteATime) < 0.001f) ||
        (noteData->get_colorType() == ColorType::ColorB &&
         std::abs(noteData->get_time() - self->prevNoteBTime) < 0.001f)) {

        return;

    }


    float timeScale = self->audioTimeSyncController->timeScale;

    auto noteCutSoundEffect = self->noteCutSoundEffectPoolContainer->Spawn();

    noteCutSoundEffect->get_transform()->SetPositionAndRotation(self->get_transform()->get_localPosition(),
                                                                Quaternion::get_identity());

    noteCutSoundEffect->didFinishEvent->Add(reinterpret_cast<INoteCutSoundEffectDidFinishEvent *>(self));

    Saber *saber = nullptr;

    if (noteData->get_colorType() == ColorType::ColorA) {

        self->prevNoteATime = noteData->get_time();

        saber = self->saberManager->get_leftSaber();

        self->prevNoteASoundEffect = noteCutSoundEffect;

    } else if (noteData->get_colorType() == ColorType::ColorB) {

        self->prevNoteBTime = noteData->get_time();

        saber = self->saberManager->get_rightSaber();

        self->prevNoteBSoundEffect = noteCutSoundEffect;

    }

    if(saber == nullptr) {
        getLogger().info("SABER IS NULLPTR AGAIN? USING RIGHT TO COMBAT.");
        saber = self->saberManager->rightSaber;
        if(noteData->get_gameplayType() == NoteData::GameplayType::Bomb) {
            self->set_enabled(false);
            return;
        }
    }

    bool flag2 = noteData->get_timeToPrevColorNote() < self->beatAlignOffset;

    AudioClip *audioClip = flag2 ? self->randomShortCutSoundPicker->PickRandomObject()
                                 : self->randomLongCutSoundPicker->PickRandomObject();

    float num = 0.9f;

    noteCutSoundEffect->Init(audioClip, noteController, (double) (noteData->get_time() / timeScale) +
                                                        self->audioTimeSyncController->get_dspTimeOffset() +
                                                        (double) self->audioTimeSyncController->get_songTimeOffset(),
                             self->beatAlignOffset, 0.15f, noteData->get_timeToPrevColorNote() / timeScale,
                             noteData->get_timeToNextColorNote() / timeScale, saber,
                             self->get_handleWrongSaberTypeAsGood(), num * cutSfxVolumeMultiplier,
                             self->useTestAudioClips, self->initData->ignoreBadCuts);

    ListW<NoteCutSoundEffect *> activeItems = self->noteCutSoundEffectPoolContainer->activeItems->get_items();

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


    if (self->noteController != noteController) {

        return;

    }


    self->noteWasCut = true;


    if (!self->ignoreBadCuts && ((!self->handleWrongSaberTypeAsGood && !noteCutInfo->get_allIsOK()) ||
                                        self->handleWrongSaberTypeAsGood &&
                                        (!noteCutInfo->get_allExceptSaberTypeIsOK() || noteCutInfo->saberTypeOK))) {

        self->audioSource->set_priority(16);

        auto clip = self->badCutRandomSoundPicker->PickRandomObject();

        self->audioSource->set_clip(clip);

        self->audioSource->set_time(0.0f);

        self->audioSource->Play();

        self->goodCut = false;

        self->audioSource->set_volume(self->badCutVolume);

        self->endDSPtime = AudioSettings::get_dspTime() +
                                  (self->audioSource->get_clip()->get_length() / self->pitch) +
                                  0.10000000149011612;

    } else {

        self->audioSource->set_priority(24);

        self->goodCut = true;

        self->audioSource->set_volume(self->goodCutVolume);

    }


    if (!self->goodCut) {

        self->get_transform()->set_position(noteCutInfo->cutPoint);

        return;

    }


    if (getTMTConfig().enableSpatialization.GetValue()) {

        self->get_transform()->set_position(noteCutInfo->cutPoint);

    }

}

MAKE_HOOK_MATCH(NCSE_LateUpdate, &NoteCutSoundEffect::LateUpdate, void, NoteCutSoundEffect *self) {


    double dspTime = AudioSettings::get_dspTime();

    if (dspTime - self->endDSPtime > 0.0) {

        self->StopPlayingAndFinish();

        return;

    }

    if (!self->noteWasCut) {

        if (dspTime > self->startDSPTime + (double) self->aheadTime - 0.05000000074505806) {

            self->audioSource->set_priority(32);

        }

        float num = self->goodCutVolume;

        if (!self->ignoreSaberSpeed) {

            num *= self->speedToVolumeCurve->Evaluate(self->saber->get_bladeSpeed()) * (1.0f -
                                                                                                      Mathf::Clamp01(
                                                                                                              (self->audioSource->get_time() /
                                                                                                               self->pitch -
                                                                                                               self->aheadTime) /
                                                                                                              self->noteMissedTimeOffset));

        }

        if (num < self->beforeCutVolume) {

            self->beforeCutVolume = std::lerp(self->beforeCutVolume, num, Time::get_deltaTime() * 4.0f);

        } else {

            self->beforeCutVolume = num;

        }

        self->audioSource->set_volume(self->beforeCutVolume * self->volumeMultiplier);

        return;

    }

    if (self->goodCut) {

        self->audioSource->set_volume(self->goodCutVolume * self->volumeMultiplier);

    }


    if (!self->noteWasCut && !getTMTConfig().staticSoundPos.GetValue()) {

//        self->get_transform()->set_position(self->saber()->get_saberBladeTopPos());

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