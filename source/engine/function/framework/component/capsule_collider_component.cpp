#include "capsule_collider_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::CapsuleColliderComponent>("CapsuleColliderComponent")
	.property("radius", &Lacrima::CapsuleColliderComponent::m_radius)
	.property("height", &Lacrima::CapsuleColliderComponent::m_height);
}

CEREAL_REGISTER_TYPE(Lacrima::CapsuleColliderComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::ColliderComponent, Lacrima::CapsuleColliderComponent)

namespace Lacrima
{

	CapsuleColliderComponent::CapsuleColliderComponent()
	{
		m_type = EColliderType::Capsule;
		m_radius = 1.0f;
		m_height = 2.0f;
	}

}