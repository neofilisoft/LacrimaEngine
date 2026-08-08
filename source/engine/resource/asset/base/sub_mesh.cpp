#include "sub_mesh.h"
#include "engine/function/global/engine_context.h"
#include "engine/resource/asset/asset_manager.h"

RTTR_REGISTRATION
{
rttr::registration::class_<Lacrima::SubMesh>("SubMesh")
	 .property("material", &Lacrima::SubMesh::m_material);
}

namespace Lacrima
{

	void SubMesh::bindRefs()
	{
		BIND_ASSET(m_material, Material)
	}

}