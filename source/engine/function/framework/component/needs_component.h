#pragma once

#include "component.h"
#include "simulation_defs.h"
#include <array>
#include <cereal/types/array.hpp>

namespace Lacrima
{
    class NeedsComponent : public Component
    {
    public:
        NeedsComponent();
        virtual ~NeedsComponent() = default;

        float getNeed(NeedId id) const;
        void setNeed(NeedId id, float value);
        void applyNeedDelta(NeedId id, float delta);

        static const std::array<NeedDefinition, kNeedCount>& getDefinitions();

    protected:
        virtual void tick(float delta_time) override;

    private:
        REGISTER_REFLECTION(Component)

        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("component", cereal::base_class<Component>(this)));
            ar(cereal::make_nvp("values", m_values));
        }

        std::array<float, kNeedCount> m_values{};
    };
}
