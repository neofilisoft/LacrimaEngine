#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <array>
#include <rttr/type.h>
#include <rttr/registration_friend.h>
#include <cereal/cereal.hpp>

namespace Lacrima
{
    class Entity;

    enum class NeedId : uint8_t
    {
        Hunger = 0,
        Bladder,
        Energy,
        Hygiene,
        Social,
        Fun,
        Comfort,
        Environment,
        Count
    };

    inline constexpr size_t kNeedCount = static_cast<size_t>(NeedId::Count);

    const char* ToString(NeedId id);

    struct NeedDefinition
    {
        float minValue = 0.0f;
        float maxValue = 100.0f;
        float convergence = 100.0f;
        float decayRatePerSecond = 0.1f;
        float failureThreshold = 0.0f;
        float autonomyWeight = 1.0f;

        RTTR_REGISTRATION_FRIEND
    };

    struct NeedSatisfaction
    {
        NeedId need;
        float satisfactionAmount = 0.0f;
    };

    enum class InteractionStepResult
    {
        Continue,
        Complete,
        Failed
    };

    struct InteractionDef
    {
        std::string name;
        float basePriority = 0.0f;
        bool playerVisible = true;

        std::function<bool(std::shared_ptr<Entity> actor, std::shared_ptr<Entity> target)> checkFunc;
        std::function<InteractionStepResult(std::shared_ptr<Entity> actor, std::shared_ptr<Entity> target, float dt)> runFunc;
    };

    struct AutonomyCandidate
    {
        const InteractionDef* def = nullptr;
        std::shared_ptr<Entity> target;
        std::vector<NeedSatisfaction> satisfies;
    };
}
