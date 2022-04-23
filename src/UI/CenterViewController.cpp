#include "UI/CenterViewController.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

DEFINE_TYPE(TooManyTweaks, CenterViewController)

static std::vector<QuestUI::SliderSetting*> ctabSliders;

namespace TooManyTweaks {
    void CenterViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace GlobalNamespace;
        using namespace QuestUI;
        using namespace UnityEngine;

        if(firstActivation) {
            static ArrayW<StringW> options(4);

            options[0] = "Controller";
            options[1] = "Gameplay";
            options[2] = "UI";
            options[3] = "Miscellaneous";

            auto tabController = BeatSaberUI::CreateTextSegmentedControl(get_transform(), {0, 0}, {80, 5.5}, options, [this](auto && PH1) { SwitchGameplayTab(std::forward<decltype(PH1)>(PH1)); });


            #pragma region Controller Tab

            controllerTab = BeatSaberUI::CreateScrollView(get_transform());
            controllerTab->SetActive(false);

            BeatSaberUI::CreateText(controllerTab->get_gameObject(), "Rumble Tweaks");

            BeatSaberUI::CreateToggle(controllerTab->get_transform(), "Enable Custom Rumble", getTMTConfig().enableCustomRumble.GetValue(), {0, 0}, [this] (bool newVal) {
                getTMTConfig().enableCustomRumble.SetValue(newVal);
                if (this->onControllerTabStateChange) {
                    this->onControllerTabStateChange(newVal);
                }
            });

            auto cutRumbleSlider = BeatSaberUI::CreateSliderSetting(controllerTab->get_transform(), "Cut Rumble Strength", 0.1, getTMTConfig().cutRumbleStrength.GetValue(), 0, 1, [](float val) {
                getTMTConfig().cutRumbleStrength.SetValue(val);
            });
            BeatSaberUI::AddHoverHint(cutRumbleSlider->get_gameObject(), "The amount to multiply rumble by when a note is cut.");
            ctabSliders.push_back(cutRumbleSlider);

            auto arcRumbleSlider = BeatSaberUI::CreateSliderSetting(controllerTab->get_transform(), "Arc Rumble Strength", 0.1, getTMTConfig().arcRumbleStrength.GetValue(), 0, 1, [](float val) {
                getTMTConfig().arcRumbleStrength.SetValue(val);
            });
            BeatSaberUI::AddHoverHint(arcRumbleSlider->get_gameObject(), "The amount to multiply rumble by when an arc is connected to the saber(s).");
            ctabSliders.push_back(arcRumbleSlider);

            auto chainRumbleSlider = BeatSaberUI::CreateSliderSetting(controllerTab->get_transform(), "Chain Rumble Strength", 0.1, getTMTConfig().chainRumbleStrength.GetValue(), 0, 1, [](float val) {
                getTMTConfig().chainRumbleStrength.SetValue(val);
            });
            BeatSaberUI::AddHoverHint(chainRumbleSlider->get_gameObject(), "The amount to multiply rumble by when a chain note is cut.");
            ctabSliders.push_back(chainRumbleSlider);

            onControllerTabStateChange = [] (bool val) {
                for (auto slider : ctabSliders) {
                    slider->slider->set_interactable(val);
                }
            };

            onControllerTabStateChange(getTMTConfig().enableCustomRumble.GetValue());

            #pragma endregion


            SwitchGameplayTab(0);
        }
    }

    void CenterViewController::SwitchGameplayTab(int idx) {
          controllerTab->SetActive(idx == 0);
//        get_transform()->Find("GameplayTabContents")->get_gameObject()->SetActive(idx == 1);
//        get_transform()->Find("UITabContents")->get_gameObject()->SetActive(idx == 2);
//        get_transform()->Find("MiscTabContents")->get_gameObject()->SetActive(idx == 3);
    }
}