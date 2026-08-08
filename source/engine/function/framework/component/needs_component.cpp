#include "needs_component.h"
#include "engine/function/framework/entity/entity.h"
#include <algorithm>
#include <cmath>

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::NeedsComponent>("NeedsComponent")
	 .property("values", &Lacrima::NeedsComponent::m_values);
}

CEREAL_REGISTER_TYPE(Lacrima::NeedsComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Component, Lacrima::NeedsComponent)

namespace Lacrima
{
    const char* ToString(NeedId id)
    {
        switch (id)
        {
            case NeedId::Hunger:      return "Hunger";
            case NeedId::Bladder:     return "Bladder";
            case NeedId::Energy:      return "Energy";
            case NeedId::Hygiene:     return "Hygiene";
            case NeedId::Social:      return "Social";
            case NeedId::Fun:         return "Fun";
            case NeedId::Comfort:     return "Comfort";
            case NeedId::Environment: return "Environment";
            default:                  return "Invalid";
        }
    }

    NeedsComponent::NeedsComponent()
    {
        // Initialize needs to middle-ground values
        m_values.fill(70.0f);
    }

    float NeedsComponent::getNeed(NeedId id) const
    {
        return m_values[static_cast<size_t>(id)];
    }

    void NeedsComponent::setNeed(NeedId id, float value)
    {
        const auto& definitions = getDefinitions();
        const auto& def = definitions[static_cast<size_t>(id)];
        m_values[static_cast<size_t>(id)] = std::clamp(value, def.minValue, def.maxValue);
    }

    void NeedsComponent::applyNeedDelta(NeedId id, float delta)
    {
        const auto& definitions = getDefinitions();
        const auto& def = definitions[static_cast<size_t>(id)];
        size_t index = static_cast<size_t>(id);
        m_values[index] = std::clamp(m_values[index] + delta, def.minValue, def.maxValue);
    }

    const std::array<NeedDefinition, kNeedCount>& NeedsComponent::getDefinitions()
    {
        static std::array<NeedDefinition, kNeedCount> definitions = []() {
            std::array<NeedDefinition, kNeedCount> defs;
            for (auto& def : defs)
            {
                def.minValue = 0.0f;
                def.maxValue = 100.0f;
                def.convergence = 100.0f;
                def.failureThreshold = 0.0f;
                def.decayRatePerSecond = 0.5f;
                def.autonomyWeight = 1.0f;
            }
            // Custom rates matching DT project
            defs[static_cast<size_t>(NeedId::Hunger)].decayRatePerSecond = 1.2f;
            defs[static_cast<size_t>(NeedId::Energy)].decayRatePerSecond = 0.8f;
            return defs;
        }();
        return definitions;
    }

    void NeedsComponent::tick(float delta_time)
    {
        const auto& definitions = getDefinitions();
        for (size_t i = 0; i < kNeedCount; ++i)
        {
            const NeedDefinition& def = definitions[i];
            float& value = m_values[i];

            const float direction = (def.convergence > value) ? 1.0f : (def.convergence < value) ? -1.0f : 0.0f;
            value += direction * def.decayRatePerSecond * delta_time;

            if (direction > 0.0f)
            {
                value = std::min(value, def.convergence);
            }
            else if (direction < 0.0f)
            {
                value = std::max(value, def.convergence);
            }

            value = std::clamp(value, def.minValue, def.maxValue);
        }
    }
}
