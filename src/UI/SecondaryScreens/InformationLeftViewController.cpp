#include "UI/SecondaryScreens/InformationLeftViewController.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Application.hpp"

#include "UI/Components/Shimmer.hpp"

DEFINE_TYPE(TooManyTweaks, InformationLeftViewController);

namespace TooManyTweaks {

#define CreateExternalButton(transform, text, tooltip, url) \
    QuestUI::BeatSaberUI::AddHoverHint(QuestUI::BeatSaberUI::CreateUIButton(transform, text, [] () { \
        Application::OpenURL(url);                                               \
    }), tooltip); \


    void
    InformationLeftViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace QuestUI;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
        using namespace HMUI;

        if (firstActivation) {
            auto scrollView = BeatSaberUI::CreateScrollView(get_transform());

            auto title = BeatSaberUI::CreateText(scrollView->get_transform(), "<size=140%>Useful Links</size>\n");
            title->set_alignment(TMPro::TextAlignmentOptions::Center);

            auto titleShimmer = title->get_gameObject()->AddComponent<Shimmer *>();
            titleShimmer->startColor = Color(245.0f / 255.0f, 115.0f / 255.0f, 34.0f / 255.0f, 1.0f);
            titleShimmer->endColor = Color(15.0f / 255.0f, 205.0f / 255.0f, 212.0f / 255.0f, 1.0f);

            CreateExternalButton(scrollView->get_transform(), "Patreon",
                                 "Join my patreon to get access to upcoming mods and updates before they release.",
                                 "https://patreon.com/cal117")
            CreateExternalButton(scrollView->get_transform(), "Discord",
                                 "Join my discord and keep updated with whats going on with my mods.",
                                 "https://discord.gg/bhGPJGbegt")
            CreateExternalButton(scrollView->get_transform(), "More Mods",
                                 "Want to see what other mods I've made? Checkout my itch.io page here.",
                                 "https://cal117.itch.io/")

            HorizontalLayoutGroup *horizontalLayoutGroup =
                    QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(
                            scrollView->get_transform());
            horizontalLayoutGroup->set_padding(RectOffset::New_ctor(8, 0, 5, -5));

            BeatSaberUI::CreateText(horizontalLayoutGroup->get_transform(),
                                    "TMT (ARR) cal117", Vector2::get_zero(),
                                    Vector2(4, 4));

        }
    }
}