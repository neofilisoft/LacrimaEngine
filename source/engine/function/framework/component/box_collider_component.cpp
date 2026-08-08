#include "box_collider_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::BoxColliderComponent>("BoxColliderComponent")
	.property("size", &Lacrima::BoxColliderComponent::m_size);
}

CEREAL_REGISTER_TYPE(Lacrima::BoxColliderComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::ColliderComponent, Lacrima::BoxColliderComponent)

namespace Lacrima
{

	BoxColliderComponent::BoxColliderComponent()
	{
		m_type = EColliderType::Box;
		m_size = glm::vec3(1.0f);
	}

}