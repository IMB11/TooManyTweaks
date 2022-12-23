#include "UI/SecondaryScreens/InformationLeftViewController.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Application.hpp"

#include "UI/Components/Shimmer.hpp"

DEFINE_TYPE(TooManyTweaks, InformationLeftViewController);

namespace TooManyTweaks {

    void
    InformationLeftViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace QuestUI;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
        using namespace HMUI;

        if (firstActivation) {
            auto scrollView = BeatSaberUI::CreateScrollView(get_transform());

            l("titlemake")

            auto title = BeatSaberUI::CreateText(scrollView->get_transform(), "<size=140%>Useful Links</size>\n");
            title->set_alignment(TMPro::TextAlignmentOptions::Center);

            l("titlemakedone")
            l("titleaddshimmer")

            auto titleShimmer = title->get_gameObject()->AddComponent<Shimmer *>();
            titleShimmer->startColor = Color(245.0f / 255.0f, 115.0f / 255.0f, 34.0f / 255.0f, 1.0f);
            titleShimmer->endColor = Color(15.0f / 255.0f, 205.0f / 255.0f, 212.0f / 255.0f, 1.0f);

            l("titleendshimmer")

//            CreateExternalButton(scrollView->get_transform(), "Patreon",
//                                 "Join my patreon to get access to upcoming mods and updates before they release.",
//                                 "https://patreon.com/cal117")

            QuestUI::BeatSaberUI::AddHoverHint(QuestUI::BeatSaberUI::CreateUIButton(scrollView->get_transform(), "Discord", [] () {
                Application::OpenURL("https://discord.gg/U9AHqcZkTE");
            }), "Join my discord and keep updated with whats going on with my mods.");

            l("titletooltip1")

            QuestUI::BeatSaberUI::AddHoverHint(QuestUI::BeatSaberUI::CreateUIButton(scrollView->get_transform(), "More Mods", [] () {
                Application::OpenURL("https://mineblock11.dev/");
            }), "Want to see what other mods I've made? Checkout my website.");

            l("titletooltip2")

            HorizontalLayoutGroup *horizontalLayoutGroup =
                    QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(
                            scrollView->get_transform());
            horizontalLayoutGroup->set_padding(RectOffset::New_ctor(8, 0, 5, -5));

            l("titlehozgroup")

            BeatSaberUI::CreateText(horizontalLayoutGroup->get_transform(),
                                    "TMT (ARR) mineblock11", Vector2::get_zero(),
                                    Vector2(4, 4));

            l("titleend!")

        }
    }
}