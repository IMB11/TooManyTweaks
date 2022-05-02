#include "TooManyTweaks.hpp"
#include "UI/CoreFlowCoordinator.hpp"

DEFINE_CONFIG(TMTConfig)

Configuration& getConfig()  {
    static Configuration config(modInfo);
    config.Load();
    return config;
};

Logger& getLogger()  {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load();
    getLogger().info("Completed setup!");
}

extern "C" void load() {
    il2cpp_functions::Init();

    getTMTConfig().Init(modInfo);

    getLogger().info("Installing hooks...");
    TooManyTweaks::Hooks::InstallHooks(getLogger());
    getLogger().info("Installed all hooks!");

    QuestUI::Register::RegisterModSettingsFlowCoordinator<TooManyTweaks::CoreFlowCoordinator*>(modInfo);
    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<TooManyTweaks::CoreFlowCoordinator*>(modInfo);

}