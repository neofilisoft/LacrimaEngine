#include "rigidbody_component.h"
#include "engine/core/base/macro.h"

RTTR_REGISTRATION
{
rttr::registration::enumeration<Lacrima::EMotionType>("EMotionType")
(
	rttr::value("Static", Lacrima::EMotionType::Static),
	rttr::value("Kinematic", Lacrima::EMotionType::Kinematic),
	rttr::value("Dynamic", Lacrima::EMotionType::Dynamic)
);

rttr::registration::class_<Lacrima::RigidbodyComponent>("RigidbodyComponent")
	.property("motion_type", &Lacrima::RigidbodyComponent::m_motion_type)
	.property("friction", &Lacrima::RigidbodyComponent::m_friction)
	.property("restitution", &Lacrima::RigidbodyComponent::m_restitution)
	.property("linear_damping", &Lacrima::RigidbodyComponent::m_linear_damping)
	.property("angular_damping", &Lacrima::RigidbodyComponent::m_angular_damping)
	.property("gravity_factor", &Lacrima::RigidbodyComponent::m_gravity_factor);
}

CEREAL_REGISTER_TYPE(Lacrima::RigidbodyComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Component, Lacrima::RigidbodyComponent)

namespace Lacrima
{
	POLYMORPHIC_DEFINITION(RigidbodyComponent)
}