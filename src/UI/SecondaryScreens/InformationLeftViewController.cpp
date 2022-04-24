#include "UI/SecondaryScreens/InformationLeftViewController.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Application.hpp"

#include "UI/Components/Shimmer.hpp"

DEFINE_TYPE(TooManyTweaks, InformationLeftViewController);

namespace TooManyTweaks {

    void InformationLeftViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace QuestUI;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
        using namespace HMUI;

        if(firstActivation) {
            auto scrollView = BeatSaberUI::CreateScrollView(get_transform());

            auto title = BeatSaberUI::CreateText(scrollView->get_transform(), "<size=140%>Premium</size>");
            title->set_alignment(TMPro::TextAlignmentOptions::Center);

            auto titleShimmer = title->get_gameObject()->AddComponent<Shimmer*>();
            titleShimmer->startColor = Color(245.0f / 255.0f, 115.0f / 255.0f, 34.0f / 255.0f, 1.0f);
            titleShimmer->endColor = Color(15.0f / 255.0f, 205.0f / 255.0f, 212.0f / 255.0f, 1.0f);

            HorizontalLayoutGroup *horizontalLayoutGroup =
                    QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(
                            scrollView->get_transform());
            horizontalLayoutGroup->set_padding(RectOffset::New_ctor(0, 0, -5, 5));



            BeatSaberUI::CreateText(horizontalLayoutGroup->get_transform(),
                                    "TooManyTweaks (ARR) cal117", Vector2::get_zero(),
                                    Vector2(4, 4));
            Button *donateButton = BeatSaberUI::CreateUIButton(
                    horizontalLayoutGroup->get_transform(), "More by cal117",
                    []() { Application::OpenURL("https://cal117.itch.io/"); });
            BeatSaberUI::SetButtonTextSize(donateButton, 3);

        }
    }
}