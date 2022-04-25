#include "TooManyTweaks.hpp"

#include "GlobalNamespace/NoteDebrisSpawner.hpp"
#include "GlobalNamespace/FlyingScoreSpawner.hpp"
#include "GlobalNamespace/BeatLineManager.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

MAKE_HOOK_MATCH(NoteDebrisHook, &NoteDebrisSpawner::SpawnDebris, void, NoteDebrisSpawner *self,
                GlobalNamespace::NoteData::GameplayType noteGameplayType, UnityEngine::Vector3 cutPoint,
                UnityEngine::Vector3 cutNormal, float saberSpeed, UnityEngine::Vector3 saberDir,
                UnityEngine::Vector3 notePos, UnityEngine::Quaternion noteRotation, UnityEngine::Vector3 noteScale,
                GlobalNamespace::ColorType colorType, float timeToNextColorNote, UnityEngine::Vector3 moveVec) {
    if (getTMTConfig().disableDebris.GetValue()) return;
    NoteDebrisHook(self, noteGameplayType, cutPoint, cutNormal, saberSpeed, saberDir, notePos, noteRotation, noteScale,
                   colorType, timeToNextColorNote, moveVec);
}

MAKE_HOOK_MATCH(SliceScoreHook, &FlyingScoreSpawner::SpawnFlyingScore, void, FlyingScoreSpawner *self,
                GlobalNamespace::IReadonlyCutScoreBuffer *cutScoreBuffer, UnityEngine::Color color) {
    if (getTMTConfig().disableSliceScore.GetValue()) return;
    SliceScoreHook(self, cutScoreBuffer, color);
}

MAKE_HOOK_MATCH(BeatLinesHook, &BeatLineManager::Start, void, BeatLineManager *self) {
    BeatLinesHook(self);

    if (getTMTConfig().disableBeatLines.GetValue()) self->set_enabled(false);
}

//MAKE_HOOK_MATCH(WallHook, &StretchableObstacle::SetSizeAndColor, void, StretchableObstacle *self,
//                float width, float height, float length, UnityEngine::Color color) {
//    WallHook(self, width, height, length, color);
//
//    if (getTMTConfig().transparentWalls.GetValue()) {
//        self->dyn__obstacleFrame()->dyn_color().dyn_a() = 0.1;
//        self->dyn__obstacleFrame()->Refresh();
//    }
//}

void InstallNoteTweakHooks(Logger &logger) {
    INSTALL_HOOK(logger, NoteDebrisHook);
    INSTALL_HOOK(logger, SliceScoreHook);
    INSTALL_HOOK(logger, BeatLinesHook);
//    INSTALL_HOOK(logger, WallHook);
}

RegisterMTHooks(InstallNoteTweakHooks)