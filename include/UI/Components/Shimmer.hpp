#pragma once

#include "TooManyTweaks.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

// clang-format off
DECLARE_CLASS_CODEGEN(TooManyTweaks, Shimmer, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_METHOD(void, Update);
                              DECLARE_INSTANCE_METHOD(void, Awake);
                              DECLARE_INSTANCE_FIELD(UnityEngine::Color, startColor);
                              DECLARE_INSTANCE_FIELD(UnityEngine::Color, endColor);
                              DECLARE_INSTANCE_FIELD(float, time);
                              DECLARE_INSTANCE_FIELD(bool, forwards);
                              DECLARE_INSTANCE_FIELD(bool, isCycling);
                              DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI * , text);

                              public:
                              custom_types::Helpers::Coroutine cycle(Color startColor, Color endColor, float cycleTime);
)
// clang-format on