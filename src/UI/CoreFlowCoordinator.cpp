#include "UI/CoreFlowCoordinator.hpp"
#include "UI/SecondaryScreens/InformationLeftViewController.hpp"
#include "UI/SecondaryScreens/ControllerTabRightViewController.hpp"

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

            centerView = QuestUI::BeatSaberUI::CreateViewController<CenterViewController *>();

            reinterpret_cast<TooManyTweaks::CenterViewController*>(centerView)->parentFlowCoordinator = this;

            ProvideInitialViewControllers(centerView, QuestUI::BeatSaberUI::CreateViewController<InformationLeftViewController*>(), QuestUI::BeatSaberUI::CreateViewController<ControllerTabRightViewController*>(), nullptr, nullptr);
        }
    }

    void CoreFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
        using namespace GlobalNamespace;
        using namespace HMUI;
        using namespace UnityEngine;

        parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
    }
}