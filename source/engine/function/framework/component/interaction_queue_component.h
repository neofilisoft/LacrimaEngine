#pragma once

#include "component.h"
#include "simulation_defs.h"
#include <deque>

namespace Lacrima
{
    struct QueuedInteraction
    {
        const InteractionDef* def = nullptr;
        std::weak_ptr<Entity> target;
        bool isAutonomous = false;

        // Custom execution state to track multi-tick behaviors in C++
        int stepCount = 0;
        float elapsed = 0.0f;
    };

    class InteractionQueueComponent : public Component
    {
    public:
        InteractionQueueComponent();
        virtual ~InteractionQueueComponent() = default;

        void push(const InteractionDef& def, std::shared_ptr<Entity> target);
        void pushAutonomous(const InteractionDef& def, std::shared_ptr<Entity> target);

        bool isEmpty() const { return m_queue.empty(); }
        size_t size() const { return m_queue.size(); }

        QueuedInteraction* Front();
        void popFront();
        void clear() { m_queue.clear(); }

        static const std::vector<InteractionDef>& getGlobalInteractions();

    protected:
        virtual void tick(float delta_time) override;

    private:
        REGISTER_REFLECTION(Component)

        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("component", cereal::base_class<Component>(this)));
        }

        void runAutonomy(std::shared_ptr<Entity> actor, std::shared_ptr<class NeedsComponent> needs);

        std::deque<QueuedInteraction> m_queue;
    };
}
