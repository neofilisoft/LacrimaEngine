#include "collider_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::ColliderComponent>("ColliderComponent")
	.property("position", &Lacrima::ColliderComponent::m_position)
	.property("rotation", &Lacrima::ColliderComponent::m_rotation);
}

CEREAL_REGISTER_TYPE(Lacrima::ColliderComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Component, Lacrima::ColliderComponent)

namespace Lacrima
{
	POLYMORPHIC_DEFINITION(ColliderComponent)
}