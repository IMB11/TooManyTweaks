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

    BETTER_ENUM(TweakCategory, char,
                Controller,
                UI,
                Gameplay,
                Miscellaneous
    )

    class Tweak {
    public:
        ~Tweak() = default;

        virtual TweakCategory getCategory() = 0;
        virtual ConfigValue getConfigValue() = 0;
        virtual void installHooks() = 0;
    };



    class Tweaks {
    private:
        inline static std::vector<Tweak*> registeredTweaks;
    public:

        template<typename T, typename = std::enable_if<std::is_base_of<Tweak, T>::value>>
        static void Add(const T& tweak) {
            registeredTweaks.push_back(*tweak);
        }

        static void InstallHooks(Logger &logger) {
            for (auto &item: registeredTweaks) {
                item->installHooks();
            }
        }

        static std::vector<Tweak*> getAllInCategory(TweakCategory category) {
            std::vector<Tweak*> _;
            std::copy_if (registeredTweaks.begin(), registeredTweaks.end(), std::back_inserter(_), [&category](Tweak *tweak) {
                return tweak->getCategory() == category;
            });
            return _;
        }
    };

    /*
     * Legacy, should be used for anything that ISN'T a tweak.
     */
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

#define DeclareTweak(tweak, ca) \
    struct __ManyTweaksDeclare#tweak {                                          \
        __ManyTweaksDeclare##tweak() { TooManyTweaks::Tweaks::Add() }  \
    };                                                                         \
    static __ManyTweaksDeclare##tweak __ManyTweaksDeclareInstance##tweak;

#define RegisterMTHook(func)                                             \
    struct __ManyTweaksRegister##func {                                          \
        __ManyTweaksRegister##func() { TooManyTweaks::Hooks::AddInstallFunc(func); }  \
    };                                                                         \
    static __ManyTweaksRegister##func __ManyTweaksRegisterInstance##func;