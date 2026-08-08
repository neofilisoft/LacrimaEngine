#include "directional_light_component.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::DirectionalLightComponent>("DirectionalLightComponent")
	.property("cascade_frustum_near", &Lacrima::DirectionalLightComponent::m_cascade_frustum_near);
}

CEREAL_REGISTER_TYPE(Lacrima::DirectionalLightComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::LightComponent, Lacrima::DirectionalLightComponent)

namespace Lacrima
{
	DirectionalLightComponent::DirectionalLightComponent()
	{
		m_intensity = 4.0f;
		m_cascade_frustum_near = 0.0f;
	}

}
