#include "UI/SecondaryScreens/InformationLeftViewController.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(TooManyTweaks, InformationLeftViewController);

namespace TooManyTweaks {
    void InformationLeftViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace QuestUI;
        using namespace UnityEngine;
        using namespace HMUI;

        if(firstActivation) {
            auto scrollView = BeatSaberUI::CreateScrollView(get_transform());

            auto title = BeatSaberUI::CreateText(scrollView->get_transform(), "<size=140%>Changelog</size>");
            title->set_alignment(TMPro::TextAlignmentOptions::Center);

            auto changelog = "<size=120%>1.0.0</size>"
                             "The initial release of TooManyTweaks, there may be some minor bugs, so please help out and report them at the GitHub repo or on discord."
                             "\n<size=110%>Added Tweaks</size>"
                             "<size=105%>Controller Tweaks</size>"
                             "- Custom Rumble Intensities"
                             "<size=95%>- Cut Rumble Strength"
                             "- Chain Rumble Strength"
                             "- Arc Rumble Strength</size>";

            auto hozLayout = BeatSaberUI::CreateHorizontalLayoutGroup(scrollView->get_transform());

            hozLayout->GetComponent<Backgroundable *>()->ApplyBackgroundWithAlpha("round-rect-panel", 0.5f);

            BeatSaberUI::CreateText(hozLayout->get_transform(), changelog);
        }

    }
}