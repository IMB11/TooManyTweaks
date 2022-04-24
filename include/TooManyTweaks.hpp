#pragma once

#include "modloader/shared/modloader.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "config-utils/shared/config-utils.hpp"
#include "custom-types/shared/macros.hpp"

#include "questui/shared/QuestUI.hpp"

#include "enum.h"

#include "TooManyTweaksConfig.hpp"

static ModInfo modInfo;

Configuration& getConfig();

// Returns a logger, useful for printing debug messages
Logger& getLogger();

template <class Type, class BaseClass>
concept CheckType = std::is_base_of<BaseClass, Type>::value;

namespace TooManyTweaks {
    class Hooks {
    private:
        inline static std::vector<void (*)(Logger &logger)> installFuncs;

    public:
        static void AddInstallFunc(void (*installFunc)(Logger &logger)) {
            installFuncs.push_back(installFunc);
        }

        static void InstallHooks(Logger &logger) {
            for (auto installFunc : installFuncs) {
                installFunc(logger);
            }
        }
    };
} // namespace ManyTweaks

#define RegisterMTHooks(func)                                             \
    struct __ManyTweaksRegister##func {                                          \
        __ManyTweaksRegister##func() { TooManyTweaks::Hooks::AddInstallFunc(func); }  \
    };                                                                         \
    static __ManyTweaksRegister##func __ManyTweaksRegisterInstance##func;