#include "UI/SecondaryScreens/InformationLeftViewController.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(TooManyTweaks, InformationLeftViewController);

namespace TooManyTweaks {
    GameObject* CreateChangelogLayout(UnityEngine::Transform* parent, std::string text) {
        using namespace QuestUI;
        using namespace UnityEngine;
        using namespace HMUI;
        using namespace UnityEngine::UI;

        auto horizon = BeatSaberUI::CreateHorizontalLayoutGroup(parent);
        auto layout = BeatSaberUI::CreateVerticalLayoutGroup(horizon->get_transform());

        auto tmproText = BeatSaberUI::CreateText(layout->get_transform(), text, false);
        tmproText->set_fontSize(tmproText->get_fontSize() * 0.65f);
        tmproText->set_alignment(TMPro::TextAlignmentOptions::_get_MidlineLeft());
        tmproText->set_enableWordWrapping(true);

        auto clayout = tmproText->get_gameObject()->AddComponent<LayoutElement*>();
        layout->set_childControlHeight(true);
        layout->set_childForceExpandHeight(true);
        layout->set_childControlWidth(true);
        layout->set_childForceExpandWidth(true);
        horizon->set_childControlWidth(true);
        horizon->set_childForceExpandWidth(true);
        clayout->set_preferredWidth(80.f);

        return horizon->get_gameObject();
    }

    void InformationLeftViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace QuestUI;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
        using namespace HMUI;

        if(firstActivation) {
            auto scrollView = BeatSaberUI::CreateScrollView(get_transform());

            auto title = BeatSaberUI::CreateText(scrollView->get_transform(), "<size=140%>Changelog</size>");
            title->set_alignment(TMPro::TextAlignmentOptions::Center);

            auto changelog = "<size=120%>1.0.0</size>\n"
                             "The initial release of TooManyTweaks, there may be some minor bugs, so please help out and report them at the GitHub repo or on discord.\n"
                             "<size=110%>Added Tweaks</size>\n"
                             "<size=105%>Controller Tweaks</size>\n"
                             "- Custom Rumble Intensities\n"
                             "<size=95%>- Cut Rumble Strength\n"
                             "- Chain Rumble Strength\n"
                             "- Arc Rumble Strength</size>\n";

            auto pLayoutGroup = BeatSaberUI::CreateVerticalLayoutGroup(scrollView->get_transform());
            pLayoutGroup->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackgroundWithAlpha("round-rect-panel", 0.5f);
            CreateChangelogLayout(pLayoutGroup->get_transform(), changelog);
        }
    }
}