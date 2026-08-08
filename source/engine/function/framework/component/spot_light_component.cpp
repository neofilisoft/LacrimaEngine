#include "spot_light_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::SpotLightComponent>("SpotLightComponent")
	 .property("inner_cone_angle", &Lacrima::SpotLightComponent::m_inner_cone_angle)
	 .property("outer_cone_angle", &Lacrima::SpotLightComponent::m_outer_cone_angle);
}

CEREAL_REGISTER_TYPE(Lacrima::SpotLightComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::PointLightComponent, Lacrima::SpotLightComponent)

namespace Lacrima
{
	SpotLightComponent::SpotLightComponent()
	{
		m_inner_cone_angle = 30.0f;
		m_outer_cone_angle = 45.0f;
	}

}