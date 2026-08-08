#include "point_light_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::PointLightComponent>("PointLightComponent")
	 .property("radius", &Lacrima::PointLightComponent::m_radius)
	 .property("linear_attenuation", &Lacrima::PointLightComponent::m_linear_attenuation)
	 .property("quadratic_attenuation", &Lacrima::PointLightComponent::m_quadratic_attenuation);
}

CEREAL_REGISTER_TYPE(Lacrima::PointLightComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::LightComponent, Lacrima::PointLightComponent)

namespace Lacrima
{
	POLYMORPHIC_DEFINITION(PointLightComponent)

	PointLightComponent::PointLightComponent()
	{
		m_radius = 64.0f;
		m_linear_attenuation = 0.14f;
		m_quadratic_attenuation = 0.07f;
	}

}