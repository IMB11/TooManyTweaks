#include "UI/CenterViewController.hpp"
#include "UI/SecondaryScreens/ControllerTabRightViewController.hpp"

#include "UnityEngine/GameObject.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

DEFINE_TYPE(TooManyTweaks, CenterViewController)

static std::vector<QuestUI::SliderSetting *> ctabSliders;

namespace TooManyTweaks {

    UnityEngine::GameObject *AdjustedScrollContainerObject(UnityEngine::GameObject *container, bool active) {
        using namespace GlobalNamespace;
        using namespace QuestUI;
        using namespace UnityEngine;

        auto extComponents = container->GetComponent<ExternalComponents *>();
        auto scrollTransform = extComponents->Get<RectTransform *>();
        scrollTransform->set_anchoredPosition(Vector2(0.0f, -5.0f));
        scrollTransform->get_gameObject()->SetActive(active);
        return scrollTransform->get_gameObject();
    }

    void CenterViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace GlobalNamespace;
        using namespace QuestUI;
        using namespace UnityEngine;

        if (firstActivation) {
            static ArrayW<StringW> options(3);

            options[0] = "Controller";
            options[1] = "Gameplay";
            options[2] = "UI";
//            options[3] = "Miscellaneous";

            BeatSaberUI::CreateTextSegmentedControl(get_transform(), {75, 7}, options, [this](auto &&PH1) {
                SwitchGameplayTab(std::forward<decltype(PH1)>(PH1));
            });

#pragma region Controller Tab

            auto _controllerTab = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

            auto rText = BeatSaberUI::CreateText(_controllerTab->get_gameObject(), "Rumble Tweaks");
            rText->set_alignment(TMPro::TextAlignmentOptions::Center);

            BeatSaberUI::CreateToggle(_controllerTab->get_transform(), "Enable Custom Rumble",
                                      getTMTConfig().enableCustomRumble.GetValue(), {0, 0}, [this](bool newVal) {
                        getTMTConfig().enableCustomRumble.SetValue(newVal);
                        for (const auto &item: this->onControllerTabRumbleStateChange) {
                            item(getTMTConfig().enableCustomRumble.GetValue());
                        }
                    });

            auto cutRumbleSlider = BeatSaberUI::CreateSliderSetting(_controllerTab->get_transform(),
                                                                    "Cut Rumble Strength", 0.1,
                                                                    getTMTConfig().cutRumbleStrength.GetValue(), 0, 5,
                                                                    [](float val) {
                                                                        getTMTConfig().cutRumbleStrength.SetValue(val);
                                                                    });
            BeatSaberUI::AddHoverHint(cutRumbleSlider->get_gameObject(),
                                      "The amount to multiply rumble by when a note is cut.");
            ctabSliders.push_back(cutRumbleSlider);

            auto arcRumbleSlider = BeatSaberUI::CreateSliderSetting(_controllerTab->get_transform(),
                                                                    "Arc Rumble Strength", 0.1,
                                                                    getTMTConfig().arcRumbleStrength.GetValue(), 0, 5,
                                                                    [](float val) {
                                                                        getTMTConfig().arcRumbleStrength.SetValue(val);
                                                                    });
            BeatSaberUI::AddHoverHint(arcRumbleSlider->get_gameObject(),
                                      "The amount to multiply rumble by when an arc is connected to the saber(s).");
            ctabSliders.push_back(arcRumbleSlider);

            auto chainRumbleSlider = BeatSaberUI::CreateSliderSetting(_controllerTab->get_transform(),
                                                                      "Chain Rumble Strength", 0.1,
                                                                      getTMTConfig().chainRumbleStrength.GetValue(), 0,
                                                                      5, [](float val) {
                        getTMTConfig().chainRumbleStrength.SetValue(val);
                    });

            BeatSaberUI::AddHoverHint(chainRumbleSlider->get_gameObject(),
                                      "The amount to multiply rumble by when a chain note is cut.");
            ctabSliders.push_back(chainRumbleSlider);

            onControllerTabRumbleStateChange.push_back([](bool val) {
                for (auto slider: ctabSliders) {
                    slider->slider->set_interactable(val);
                }
            });

            for (const auto &item: onControllerTabRumbleStateChange) {
                item(getTMTConfig().enableCustomRumble.GetValue());
            }

            controllerTab = AdjustedScrollContainerObject(_controllerTab, true);

#pragma endregion

#pragma region Gameplay Tab

            auto _gameplayTab = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());


            auto nText = BeatSaberUI::CreateText(_gameplayTab->get_gameObject(), "Note Tweaks");
            nText->set_alignment(TMPro::TextAlignmentOptions::Center);

            BeatSaberUI::AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().disableDebris),
                                      "Disables debris when you cut a note.");
            BeatSaberUI::AddHoverHint(
                    AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().disableSliceScore),
                    "Hides the slice score that spawns when you cut a note.");
            BeatSaberUI::AddHoverHint(
                    AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().disableBeatLines),
                    "Hides the extremely bright light that appears on the floor when a note spawns");

//            auto wText = BeatSaberUI::CreateText(_gameplayTab->get_gameObject(), "Wall Tweaks");
//            wText->set_alignment(TMPro::TextAlignmentOptions::Center);
//
//            BeatSaberUI::AddHoverHint(
//                    AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().transparentWalls),
//                    "Makes walls transparent, useful for recording videos and streaming.");

            gameplayTab = AdjustedScrollContainerObject(_gameplayTab, false);
#pragma endregion

#pragma region UI Tab

            auto _uiTab = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());


            uiTab = AdjustedScrollContainerObject(_uiTab, false);
#pragma endregion
        }
    }

    void CenterViewController::SwitchGameplayTab(int idx) {
        using namespace QuestUI;

        switch (idx) {
            case 0:
                parentFlowCoordinator->SetRightScreenViewController(
                        BeatSaberUI::CreateViewController<ControllerTabRightViewController *>(),
                        AnimationType::_get_In());
                break;
            default:
                parentFlowCoordinator->SetRightScreenViewController(nullptr, AnimationType::_get_Out());
                break;
        }


        controllerTab->SetActive(idx == 0);
        gameplayTab->SetActive(idx == 1);
        uiTab->SetActive(idx == 2);
//        get_transform()->Find("MiscTabContents")->get_gameObject()->SetActive(idx == 3);
    }
}