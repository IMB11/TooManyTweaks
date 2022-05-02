#include "UI/SecondaryScreens/ControllerTabRightViewController.hpp"

DEFINE_TYPE(TooManyTweaks, ControllerTabRightViewController);

using namespace Libraries::HM::HMLib::VR;

namespace TooManyTweaks {
    void ControllerTabRightViewController::testRumble(HapticPresetSO *preset, GlobalNamespace::SaberType type) {
        using namespace UnityEngine;
        using namespace GlobalNamespace;

        auto instance = Resources::FindObjectsOfTypeAll<HapticFeedbackController *>().get(0);

        if (instance) {
            instance->PlayHapticFeedback(SaberTypeExtensions::Node(type), preset);
            instance->PlayHapticFeedback(SaberTypeExtensions::Node(type), preset);
        }
    }

    void ControllerTabRightViewController::DidActivate(bool firstActivation, bool addedToHierarchy,
                                                       bool screenSystemEnabling) {
        using namespace GlobalNamespace;
        using namespace QuestUI;
        using namespace UnityEngine;

        if (normalPreset == nullptr) {
            normalPreset = ScriptableObject::CreateInstance<HapticPresetSO *>();
        }

        if (weakPreset == nullptr) {
            weakPreset = ScriptableObject::CreateInstance<HapticPresetSO *>();
        }

        if (arcPreset == nullptr) {
            arcPreset = ScriptableObject::CreateInstance<HapticPresetSO *>();
            arcPreset->dyn__continuous() = true;
        }

        if (firstActivation) {
            auto scrollView = BeatSaberUI::CreateScrollView(get_transform());

            auto title = BeatSaberUI::CreateText(scrollView->get_transform(), "<size=120%>Utilities</size>");
            title->set_alignment(TMPro::TextAlignmentOptions::Center);

            std::vector<UI::Button *> allButtons;

//            auto texts = BeatSaberUI::CreateHorizontalLayoutGroup(scrollView->get_transform());

            auto cutRumbleButtons = BeatSaberUI::CreateHorizontalLayoutGroup(scrollView->get_transform());

            allButtons.push_back(
                    BeatSaberUI::CreateUIButton(cutRumbleButtons->get_transform(), "Left Cut Rumble Test", [this]() {
                        normalPreset->dyn__duration() = std::min(0.2f, DURATION_NORMAL *
                                                                       getTMTConfig().cutRumbleStrength.GetValue());
                        normalPreset->dyn__strength() =
                                STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().cutRumbleStrength.GetValue() * 1.2f);

                        testRumble(normalPreset, SaberType::_get_SaberA());
                    }));

            allButtons.push_back(
                    BeatSaberUI::CreateUIButton(cutRumbleButtons->get_transform(), "Right Cut Rumble Test", [this]() {
                        weakPreset->dyn__duration() = std::min(0.2f, DURATION_NORMAL *
                                                                     getTMTConfig().cutRumbleStrength.GetValue());
                        weakPreset->dyn__strength() =
                                STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().cutRumbleStrength.GetValue() * 1.2f);

                        testRumble(weakPreset, SaberType::_get_SaberB());
                    }));

            auto chainRumbleButtons = BeatSaberUI::CreateHorizontalLayoutGroup(scrollView->get_transform());

            allButtons.push_back(
                    BeatSaberUI::CreateUIButton(chainRumbleButtons->get_transform(), "Left Chain Rumble Test",
                                                [this]() {
                                                    normalPreset->dyn__duration() = std::min(0.2f, DURATION_WEAK *
                                                                                                   getTMTConfig().chainRumbleStrength.GetValue());
                                                    normalPreset->dyn__strength() = STRENGTH_WEAK * std::min(1.0f,
                                                                                                             getTMTConfig().chainRumbleStrength.GetValue() *
                                                                                                             1.2f);

                                                    testRumble(normalPreset, SaberType::_get_SaberA());
                                                }));

            allButtons.push_back(
                    BeatSaberUI::CreateUIButton(chainRumbleButtons->get_transform(), "Right Chain Rumble Test",
                                                [this]() {
                                                    weakPreset->dyn__duration() = std::min(0.2f, DURATION_WEAK *
                                                                                                 getTMTConfig().chainRumbleStrength.GetValue());
                                                    weakPreset->dyn__strength() = STRENGTH_WEAK * std::min(1.0f,
                                                                                                           getTMTConfig().chainRumbleStrength.GetValue() *
                                                                                                           1.2f);

                                                    testRumble(weakPreset, SaberType::_get_SaberB());
                                                }));

            auto arcRumbleButtons = BeatSaberUI::CreateHorizontalLayoutGroup(scrollView->get_transform());

            allButtons.push_back(
                    BeatSaberUI::CreateUIButton(arcRumbleButtons->get_transform(), "Left Arc Rumble Test", [this]() {
                        normalPreset->dyn__duration() = std::min(0.2f, DURATION_WEAK *
                                                                       getTMTConfig().chainRumbleStrength.GetValue());
                        normalPreset->dyn__strength() =
                                STRENGTH_WEAK * std::min(1.0f, getTMTConfig().chainRumbleStrength.GetValue() * 1.2f);

                        testRumble(normalPreset, SaberType::_get_SaberA());
                    }));

            allButtons.push_back(
                    BeatSaberUI::CreateUIButton(arcRumbleButtons->get_transform(), "Right Arc Rumble Test", [this]() {
                        weakPreset->dyn__duration() = std::min(0.2f, DURATION_WEAK *
                                                                     getTMTConfig().chainRumbleStrength.GetValue());
                        weakPreset->dyn__strength() =
                                STRENGTH_WEAK * std::min(1.0f, getTMTConfig().chainRumbleStrength.GetValue() * 1.2f);

                        testRumble(weakPreset, SaberType::_get_SaberB());
                    }));

//            arcPreset->dyn__duration() = std::min(0.2f, ARC_DURATION_NORMAL * getTMTConfig().arcRumbleStrength.GetValue());
//                arcPreset->dyn__strength() = ARC_STRENGTH_NORMAL * std::min(1.0f, getTMTConfig().arcRumbleStrength.GetValue() * 1.2f);
//
//                testRumble(arcPreset);
        }
    }
}