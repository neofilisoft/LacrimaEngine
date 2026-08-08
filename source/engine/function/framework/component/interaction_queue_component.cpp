#include "interaction_queue_component.h"
#include "needs_component.h"
#include "engine/function/framework/entity/entity.h"
#include "engine/core/base/macro.h"
#include <algorithm>
#include <cmath>
#include <limits>

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::InteractionQueueComponent>("InteractionQueueComponent");
}

CEREAL_REGISTER_TYPE(Lacrima::InteractionQueueComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Component, Lacrima::InteractionQueueComponent)

namespace Lacrima
{
    float ComputeUrgency(float currentValue, const NeedDefinition& definition)
    {
        const float totalRange = std::abs(definition.convergence - definition.failureThreshold);
        if (totalRange < 1e-6f)
        {
            return 0.0f;
        }

        const float distanceFromConvergence = std::abs(currentValue - definition.convergence);
        const float normalized = std::clamp(distanceFromConvergence / totalRange, 0.0f, 1.0f);

        return normalized * normalized;
    }

    InteractionQueueComponent::InteractionQueueComponent()
    {
    }

    void InteractionQueueComponent::push(const InteractionDef& def, std::shared_ptr<Entity> target)
    {
        QueuedInteraction q;
        q.def = &def;
        q.target = target;
        q.isAutonomous = false;
        m_queue.push_back(q);
    }

    void InteractionQueueComponent::pushAutonomous(const InteractionDef& def, std::shared_ptr<Entity> target)
    {
        QueuedInteraction q;
        q.def = &def;
        q.target = target;
        q.isAutonomous = true;
        m_queue.push_back(q);
    }

    QueuedInteraction* InteractionQueueComponent::Front()
    {
        return m_queue.empty() ? nullptr : &m_queue.front();
    }

    void InteractionQueueComponent::popFront()
    {
        if (!m_queue.empty())
        {
            m_queue.pop_front();
        }
    }

    const std::vector<InteractionDef>& InteractionQueueComponent::getGlobalInteractions()
    {
        static std::vector<InteractionDef> globals = []() {
            std::vector<InteractionDef> list;

            // Rest Interaction
            InteractionDef rest;
            rest.name = "Rest";
            rest.basePriority = 0.0f;
            rest.playerVisible = true;
            rest.checkFunc = [](std::shared_ptr<Entity> actor, std::shared_ptr<Entity> target) {
                return actor != nullptr;
            };
            rest.runFunc = [](std::shared_ptr<Entity> actor, std::shared_ptr<Entity> target, float dt) {
                auto needs = actor->getComponent<NeedsComponent>("NeedsComponent");
                if (needs)
                {
                    needs->applyNeedDelta(NeedId::Energy, 8.0f);
                }
                return InteractionStepResult::Complete;
            };
            list.push_back(rest);

            // GrabASnack Interaction
            InteractionDef snack;
            snack.name = "GrabASnack";
            snack.basePriority = 0.0f;
            snack.playerVisible = true;
            snack.checkFunc = [](std::shared_ptr<Entity> actor, std::shared_ptr<Entity> target) {
                return actor != nullptr;
            };
            snack.runFunc = [](std::shared_ptr<Entity> actor, std::shared_ptr<Entity> target, float dt) {
                auto queue = actor->getComponent<InteractionQueueComponent>("InteractionQueueComponent");
                if (!queue)
                {
                    return InteractionStepResult::Failed;
                }

                auto front = queue->Front();
                if (!front)
                {
                    return InteractionStepResult::Failed;
                }

                // First tick returns Continue
                if (front->stepCount == 0)
                {
                    front->stepCount++;
                    return InteractionStepResult::Continue;
                }

                // Second tick applies satisfaction and completes
                auto needs = actor->getComponent<NeedsComponent>("NeedsComponent");
                if (needs)
                {
                    needs->applyNeedDelta(NeedId::Hunger, 6.0f);
                }
                return InteractionStepResult::Complete;
            };
            list.push_back(snack);

            return list;
        }();

        return globals;
    }

    void InteractionQueueComponent::tick(float delta_time)
    {
        auto actor = std::static_pointer_cast<Entity>(getParent().lock());
        if (!actor)
        {
            return;
        }

        if (isEmpty())
        {
            auto needs = actor->getComponent<NeedsComponent>("NeedsComponent");
            if (needs)
            {
                runAutonomy(actor, needs);
            }
        }

        if (!isEmpty())
        {
            QueuedInteraction& front = m_queue.front();
            if (front.def && front.def->runFunc)
            {
                auto target = front.target.lock();
                InteractionStepResult result = front.def->runFunc(actor, target, delta_time);
                if (result == InteractionStepResult::Complete || result == InteractionStepResult::Failed)
                {
                    popFront();
                }
            }
            else
            {
                popFront();
            }
        }
    }

    void InteractionQueueComponent::runAutonomy(std::shared_ptr<Entity> actor, std::shared_ptr<NeedsComponent> needs)
    {
        const auto& globals = getGlobalInteractions();
        std::vector<AutonomyCandidate> candidates;

        for (const auto& def : globals)
        {
            if (def.checkFunc && !def.checkFunc(actor, actor))
            {
                continue;
            }

            AutonomyCandidate candidate;
            candidate.def = &def;
            candidate.target = actor;

            if (def.name == "Rest")
            {
                candidate.satisfies = { { NeedId::Energy, 8.0f } };
            }
            else if (def.name == "GrabASnack")
            {
                candidate.satisfies = { { NeedId::Hunger, 6.0f } };
            }

            candidates.push_back(candidate);
        }

        const AutonomyCandidate* bestCandidate = nullptr;
        float bestScore = -std::numeric_limits<float>::infinity();

        const auto& definitions = NeedsComponent::getDefinitions();

        for (const auto& candidate : candidates)
        {
            float score = candidate.def->basePriority;

            for (const auto& satisfaction : candidate.satisfies)
            {
                size_t needIndex = static_cast<size_t>(satisfaction.need);
                const auto& def = definitions[needIndex];
                float currentValue = needs->getNeed(satisfaction.need);
                float urgency = ComputeUrgency(currentValue, def);

                score += def.autonomyWeight * urgency;
            }

            if (score > bestScore)
            {
                bestScore = score;
                bestCandidate = &candidate;
            }
        }

        if (bestCandidate && bestCandidate->def)
        {
            pushAutonomous(*bestCandidate->def, bestCandidate->target);
        }
    }
}
