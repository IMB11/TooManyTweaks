#include "TooManyTweaks.hpp"
#include "UI/CoreFlowCoordinator.hpp"

Logger& getLogger()  {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getTMTConfig().Init(modInfo);
    getLogger().info("Completed setup!");
}

extern "C" void load() {
    il2cpp_functions::Init();

    custom_types::Register::AutoRegister();

    getLogger().info("Installing hooks...");
    TooManyTweaks::Hooks::InstallHooks(getLogger());
    getLogger().info("Installed all hooks!");

    QuestUI::Register::RegisterModSettingsFlowCoordinator<TooManyTweaks::CoreFlowCoordinator*>(modInfo);
    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<TooManyTweaks::CoreFlowCoordinator*>(modInfo);

}