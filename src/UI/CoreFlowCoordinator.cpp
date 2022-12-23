#include "UI/CoreFlowCoordinator.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

DEFINE_TYPE(TooManyTweaks, CoreFlowCoordinator);

namespace TooManyTweaks {
    void CoreFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        l("CoreFlowCoord start")
        using namespace GlobalNamespace;
        using namespace HMUI;
        using namespace UnityEngine;

        if (firstActivation) {
            l("CoreFlowCoord title")
            SetTitle("TooManyTweaks", ViewController::AnimationType::Out);

            set_showBackButton(true);

            l("CoreFlowCoord makecenter")
            centerView = QuestUI::BeatSaberUI::CreateViewController<CenterViewController *>();

            l("CoreFlowCoord cast center view")
            reinterpret_cast<TooManyTweaks::CenterViewController *>(centerView)->parentFlowCoordinator = this;

            l("CoreFlowCoord create infomation")
            infoView = QuestUI::BeatSaberUI::CreateViewController<InformationLeftViewController *>();

            l("CoreFlowCoord create controller")
            controllerView = QuestUI::BeatSaberUI::CreateViewController<ControllerTabRightViewController *>();

            l("CoreFlowCoord provide")
            ProvideInitialViewControllers(centerView,
                                          infoView,
                                          controllerView,
                                          nullptr, nullptr);
        }
    }

    void CoreFlowCoordinator::BackButtonWasPressed(HMUI::ViewController *topViewController) {
        using namespace GlobalNamespace;
        using namespace HMUI;
        using namespace UnityEngine;

        l("CoreFlowCoord dismiss")
        parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr,
                                                      false);
    }
}