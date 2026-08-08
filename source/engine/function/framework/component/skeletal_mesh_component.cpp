#include "skeletal_mesh_component.h"
#include "engine/function/global/engine_context.h"
#include "engine/resource/asset/asset_manager.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::SkeletalMeshComponent>("SkeletalMeshComponent")
	 .property("skeletal_mesh", &Lacrima::SkeletalMeshComponent::m_skeletal_mesh);
}

CEREAL_REGISTER_TYPE(Lacrima::SkeletalMeshComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Component, Lacrima::SkeletalMeshComponent)

namespace Lacrima
{
	void SkeletalMeshComponent::setSkeletalMesh(std::shared_ptr<SkeletalMesh>& skeletal_mesh)
	{
		REF_ASSET(m_skeletal_mesh, skeletal_mesh)
	}

	void SkeletalMeshComponent::bindRefs()
	{
		BIND_ASSET(m_skeletal_mesh, SkeletalMesh)
	}

}