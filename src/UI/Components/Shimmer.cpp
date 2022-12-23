#include "UI/Components/Shimmer.hpp"

#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/Time.hpp"

DEFINE_TYPE(TooManyTweaks, Shimmer);

custom_types::Helpers::Coroutine TooManyTweaks::Shimmer::cycle(UnityEngine::Color startColor, UnityEngine::Color endColor, float cycleTime) {
    isCycling = true;
    float currentTime = 0;
    while (currentTime < cycleTime) {
        currentTime += UnityEngine::Time::get_deltaTime();
        float t = currentTime / cycleTime;
        UnityEngine::Color currentColor = UnityEngine::Color::Lerp(startColor, endColor, t);
        text->set_color(currentColor);
        co_yield nullptr;
    }
    isCycling = false;
    forwards = !forwards;
    co_return;
}

void TooManyTweaks::Shimmer::Awake() {
    text = GetComponent<TMPro::TextMeshProUGUI *>();
    time = 1.5f;
    forwards = true;
    isCycling = false;
}

void TooManyTweaks::Shimmer::Update() {
    if (!isCycling) {
        if (forwards)
            StartCoroutine(custom_types::Helpers::CoroutineHelper::New(cycle(startColor, endColor, time)));
        else
            StartCoroutine(custom_types::Helpers::CoroutineHelper::New(cycle(endColor, startColor, time)));
    }
}
