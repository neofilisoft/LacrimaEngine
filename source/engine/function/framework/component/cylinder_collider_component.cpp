#include "cylinder_collider_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::CylinderColliderComponent>("CylinderColliderComponent")
	.property("radius", &Lacrima::CylinderColliderComponent::m_radius)
	.property("height", &Lacrima::CylinderColliderComponent::m_height);
}

CEREAL_REGISTER_TYPE(Lacrima::CylinderColliderComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::ColliderComponent, Lacrima::CylinderColliderComponent)

namespace Lacrima
{
	
	CylinderColliderComponent::CylinderColliderComponent()
	{
		m_type = EColliderType::Cylinder;
		m_radius = 1.0f;
		m_height = 2.0f;
	}

}