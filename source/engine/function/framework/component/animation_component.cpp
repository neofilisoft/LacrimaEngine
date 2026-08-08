#include "animation_component.h"
#include "engine/function/global/engine_context.h"
#include "engine/resource/asset/asset_manager.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::AnimationComponent>("AnimationComponent")
	 .property("animations", &Lacrima::AnimationComponent::m_animations);
}

CEREAL_REGISTER_TYPE(Lacrima::AnimationComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Component, Lacrima::AnimationComponent)

namespace Lacrima
{
	void AnimationComponent::addAnimation(std::shared_ptr<Animation>& animation)
	{
		m_animations.push_back({});
		uint32_t index = m_animations.size() - 1;
		REF_ASSET_ELEM(m_animations[index], std::to_string(index), animation)
	}

	void AnimationComponent::bindRefs()
	{
		for (auto iter : m_ref_urls)
		{
			m_animations.push_back({});
			uint32_t index = m_animations.size() - 1;
			BIND_ASSET_ELEM(m_animations[index], std::to_string(index), Animation)
		}
	}

}