#include "sphere_collider_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::SphereColliderComponent>("SphereColliderComponent")
	.property("radius", &Lacrima::SphereColliderComponent::m_radius);
}

CEREAL_REGISTER_TYPE(Lacrima::SphereColliderComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::ColliderComponent, Lacrima::SphereColliderComponent)

namespace Lacrima
{

	SphereColliderComponent::SphereColliderComponent()
	{
		m_type = EColliderType::Sphere;
		m_radius = 1.0f;
	}

}