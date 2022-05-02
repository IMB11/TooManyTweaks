#pragma once

#include "modloader/shared/modloader.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "config-utils/shared/config-utils.hpp"
#include "custom-types/shared/macros.hpp"

#include "questui/shared/QuestUI.hpp"

#include "TooManyTweaksConfig.hpp"

static ModInfo modInfo;

Configuration &getConfig();

// Returns a logger, useful for printing debug messages
Logger &getLogger();

#define l(m) getLogger().info("%s", m);

namespace TooManyTweaks {

//    /// @brief Resolves the provided icall, throwing an il2cpp_utils::RunMethodException with backtrace information if failed.
//    /// Does NOT cache the resolved method pointer.
//    /// Also does NOT perform any type checking of parameters, so make sure you check your parameters and return types!
//    /// @tparam R The return type of the function to resolve
//    /// @tparam TArgs The arguments of the function to resolve
//    /// @param icallName The name of the icall to resolve
//    /// @return The resolved function pointer, will always be valid or throws an il2cpp_utils::RunMethodException.
//    template<class R, class... TArgs>
//    function_ptr_t<R, TArgs...> resolve_icall(std::string_view icallName) {
//        il2cpp_functions::Init();
//        auto out = reinterpret_cast<function_ptr_t<R, TArgs...>>(il2cpp_functions::resolve_icall(icallName.data()));
//        if (!out) {
//            throw il2cpp_utils::RunMethodException(string_format("Failed to resolve_icall for icall: %s!", icallName.data()), nullptr);
//        }
//        return out;
//    }

    class Hooks {
    private:
        inline static std::vector<void (*)(Logger &logger)> installFuncs;

    public:
        static void AddInstallFunc(void (*installFunc)(Logger &logger)) {
            installFuncs.push_back(installFunc);
        }

        static void InstallHooks(Logger &logger) {
            for (auto installFunc: installFuncs) {
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