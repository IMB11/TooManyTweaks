#include "UI/CoreFlowCoordinator.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

DEFINE_TYPE(TooManyTweaks, CoreFlowCoordinator);

namespace TooManyTweaks {
    void CoreFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        using namespace GlobalNamespace;
        using namespace HMUI;
        using namespace UnityEngine;

        if (firstActivation) {
            SetTitle("TooManyTweaks", ViewController::AnimationType::Out);
            showBackButton = true;

            centerView = QuestUI::BeatSaberUI::CreateViewController<CenterViewController*>();

            ProvideInitialViewControllers(centerView, nullptr, nullptr, nullptr, nullptr);
        }
    }

    void CoreFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
        using namespace GlobalNamespace;
        using namespace HMUI;
        using namespace UnityEngine;

        parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
    }
}