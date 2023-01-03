#include "UI/CenterViewController.hpp"
#include "UI/SecondaryScreens/ControllerTabRightViewController.hpp"

#include "UnityEngine/GameObject.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

#include "System/Action_2.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "HMUI/SegmentedControl.hpp"

#define MakeADelegate(DelegateType, varName) (custom_types::MakeDelegate<DelegateType>(varName))

#include "custom-types/shared/delegate.hpp"

DEFINE_TYPE(TooManyTweaks, CenterViewController)

static std::vector<QuestUI::SliderSetting *> ctabSliders;

QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControlFIX(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked)
{
    static SafePtrUnity<HMUI::TextSegmentedControl> segmentedControlTemplate;
    if (!segmentedControlTemplate)
        segmentedControlTemplate = UnityEngine::Resources::FindObjectsOfTypeAll<HMUI::TextSegmentedControl*>().FirstOrDefault([](auto x){
            return x->get_name() == "TextSegmentedControl";
        });

    auto segmentedControlObj = UnityEngine::Object::Instantiate(segmentedControlTemplate->get_gameObject(), parent, false);
    segmentedControlObj->SetActive(false);
    static ConstString QuestUITextSegmentedControl("QuestUITextSegmentedControl");
    segmentedControlObj->set_name(QuestUITextSegmentedControl);
    auto rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(segmentedControlObj->get_transform());
    rectTransform->set_sizeDelta(sizeDelta);
    rectTransform->set_anchoredPosition(anchoredPosition);


    UnityEngine::Object::DestroyImmediate(segmentedControlObj->GetComponent<HMUI::TextSegmentedControl*>());
    auto control = segmentedControlObj->AddComponent<HMUI::SegmentedControl*>();
    auto result = segmentedControlObj->AddComponent<QuestUI::CustomTextSegmentedControlData*>();

    result->firstCellPrefab = segmentedControlTemplate->firstCellPrefab;
    result->lastCellPrefab = segmentedControlTemplate->lastCellPrefab;
    result->middleCellPrefab = segmentedControlTemplate->middleCellPrefab;
    result->singleCellPrefab = segmentedControlTemplate->singleCellPrefab;

    result->segmentedControl = control;
    control->dataSource = reinterpret_cast<HMUI::SegmentedControl::IDataSource*>(result);

    if (onCellWithIdxClicked)
    {
        using DelegateType = System::Action_2<HMUI::SegmentedControl*, int>*;
        std::function<void(HMUI::SegmentedControl*, int)> fun = [onCellWithIdxClicked](HMUI::SegmentedControl* cell, int idx){ onCellWithIdxClicked(idx); };
        auto delegate = MakeADelegate(DelegateType, fun);
        control->add_didSelectCellEvent(delegate);
    }

    int childCount = result->get_transform()->get_childCount();
    for (int i = 0; i < childCount; i++)
    {
        UnityEngine::Object::DestroyImmediate(result->get_transform()->GetChild(0)->get_gameObject());
    }

    result->set_texts(values);

    segmentedControlObj->SetActive(true);
    return result;
}

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
        using namespace QuestUI::BeatSaberUI;
        using namespace UnityEngine;

        l("activate")

        if (firstActivation) {
            l("activfirst")
            static ArrayW<StringW> options(3);
            l("createdArrayW")

            options[0] = "Controller";
            l("createdArrayW0")
            options[1] = "Gameplay";
            l("createdArrayW1")
            options[2] = "UI";
            l("createdArrayW2")
//            options[3] = "Miscellaneous";

            CreateTextSegmentedControlFIX(this->get_transform(), {0, 0}, Vector2(75, 7), options, [this](int PH1) {
                this->selectedTab = PH1;
                SwitchGameplayTab(PH1);
            });
            l("createdSegtab")

            l("tabmade")

#pragma region Controller Tab

            l("ctrltab")

            auto _controllerTab = CreateScrollableSettingsContainer(get_transform());

            auto rText = CreateText(_controllerTab->get_gameObject(), "Rumble Tweaks");
            rText->set_alignment(TMPro::TextAlignmentOptions::Center);

            CreateToggle(_controllerTab->get_transform(), "Enable Custom Rumble",
                         getTMTConfig().enableCustomRumble.GetValue(), {0, 0}, [this](bool newVal) {
                        getTMTConfig().enableCustomRumble.SetValue(newVal);
                        for (const auto &item: this->onControllerTabRumbleStateChange) {
                            item(getTMTConfig().enableCustomRumble.GetValue());
                        }
                    });

            auto cutRumbleSlider = CreateSliderSetting(_controllerTab->get_transform(),
                                                       "Cut Rumble Strength", 0.1,
                                                       getTMTConfig().cutRumbleStrength.GetValue(), 0, 5,
                                                       [](float val) {
                                                           getTMTConfig().cutRumbleStrength.SetValue(val);
                                                       });
            AddHoverHint(cutRumbleSlider->get_gameObject(),
                         "The amount to multiply rumble by when a note is cut.");
            ctabSliders.push_back(cutRumbleSlider);

            auto arcRumbleSlider = CreateSliderSetting(_controllerTab->get_transform(),
                                                       "Arc Rumble Strength", 0.1,
                                                       getTMTConfig().arcRumbleStrength.GetValue(), 0, 5,
                                                       [](float val) {
                                                           getTMTConfig().arcRumbleStrength.SetValue(val);
                                                       });
            AddHoverHint(arcRumbleSlider->get_gameObject(),
                         "The amount to multiply rumble by when an arc is connected to the saber(s).");
            ctabSliders.push_back(arcRumbleSlider);

            auto chainRumbleSlider = CreateSliderSetting(_controllerTab->get_transform(),
                                                         "Chain Rumble Strength", 0.1,
                                                         getTMTConfig().chainRumbleStrength.GetValue(), 0,
                                                         5, [](float val) {
                        getTMTConfig().chainRumbleStrength.SetValue(val);
                    });

            AddHoverHint(chainRumbleSlider->get_gameObject(),
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

            l("gampltab")

            auto _gameplayTab = CreateScrollableSettingsContainer(get_transform());

            auto nText = CreateText(_gameplayTab->get_transform(), "Note Tweaks");
            nText->set_alignment(TMPro::TextAlignmentOptions::Center);

            AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().disableDebris),
                         "Disables debris when you cut a note.");
            AddHoverHint(
                    AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().disableSliceScore),
                    "Hides the slice score that spawns when you cut a note.");
            AddHoverHint(
                    AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().disableBeatLines),
                    "Hides the extremely bright light that appears on the floor when a note spawns");

            auto sText = CreateText(_gameplayTab->get_transform(), "Hitsound Tweaks");
            sText->set_alignment(TMPro::TextAlignmentOptions::Center);
            AddHoverHint(sText->get_transform(), "Original PC Mod by GalaxyMaster");

            AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().enableHitsoundTweaks);

            AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().ignoreSaberSpeed),
                         "Should the sound be influenced by the speed of the saber when slicing?");

            AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().ignoreBadCuts),
                         "Should the bad cut hitsound be disabled?");

            AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().staticSoundPos), "Should the hitsound play at the slice position, or at the player position?");

            AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().enableSpatialization), "Should sound be spatialized? (3D audio)");

            AddHoverHint(AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().enableChainLinkHitSounds), "Enable Hitsounds for the small notes on the new chain notes? (sliders)");

            auto randomPitchMinSlider = CreateSliderSetting(_gameplayTab->get_transform(),
                                                       "Random Pitch Min", 0.1,
                                                       getTMTConfig().randomPitchMin.GetValue(), 0, 5,
                                                       [](float val) {
                                                           getTMTConfig().randomPitchMin.SetValue(val);
                                                       });

            AddHoverHint(randomPitchMinSlider, "The minimum pitch a hitsound can be.");

            auto randomPitchMaxSlider = CreateSliderSetting(_gameplayTab->get_transform(),
                                                      "Random Pitch Max", 0.1,
                                                      getTMTConfig().randomPitchMax.GetValue(), 0, 5,
                                                      [](float val) {
                                                          getTMTConfig().randomPitchMax.SetValue(val);
                                                      });

            AddHoverHint(randomPitchMaxSlider, "The maximum pitch a hitsound can be.");

            auto chainLinkVolumeMultiplier = CreateSliderSetting(_gameplayTab->get_transform(),
                                                            "Chain Link Volume Multiplier", 0.1,
                                                            getTMTConfig().chainLinkVolMultiplier.GetValue(), 0, 5,
                                                            [](float val) {
                                                                getTMTConfig().chainLinkVolMultiplier.SetValue(val);
                                                            });
            AddHoverHint(chainLinkVolumeMultiplier, "The multiplier each link on a chain note (slider) has towards the hitsound.");

//            auto wText = BeatSaberUI::CreateText(_gameplayTab->get_gameObject(), "Wall Tweaks");
//            wText->set_alignment(TMPro::TextAlignmentOptions::Center);
//
//            BeatSaberUI::AddHoverHint(
//                    AddConfigValueToggle(_gameplayTab->get_transform(), getTMTConfig().transparentWalls),
//                    "Makes walls transparent, useful for recording videos and streaming.");

            gameplayTab = AdjustedScrollContainerObject(_gameplayTab, false);
#pragma endregion

#pragma region UI Tab

            l("uitab")

            auto _uiTab = CreateScrollableSettingsContainer(get_transform());

            AddHoverHint(AddConfigValueToggle(_uiTab->get_transform(), getTMTConfig().noComboBreakEffect),
                         "Removes the annoying combo break effect that can distract some players.");
            AddHoverHint(AddConfigValueToggle(_uiTab->get_transform(), getTMTConfig().noHealthAndSafety),
                         "Removes the health and safety warnings at the start of the game.");
            AddHoverHint(AddConfigValueToggle(_uiTab->get_transform(), getTMTConfig().noPromo), "Removes the promotional banner from the screen.");

            uiTab = AdjustedScrollContainerObject(_uiTab, false);
#pragma endregion

            if(this->selectedTab != 0) {
                this->selectedTab = 0;
                SwitchGameplayTab(0);
            }
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