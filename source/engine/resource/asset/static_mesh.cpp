#include "static_mesh.h"

CEREAL_REGISTER_TYPE(Lacrima::StaticMesh)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Lacrima::Asset, Lacrima::StaticMesh)

namespace Lacrima
{

	void StaticMesh::inflate()
	{
		calcBoundingBox();

		VulkanUtil::createVertexBuffer(m_vertices.size() * sizeof(m_vertices[0]), m_vertices.data(), m_vertex_buffer);
		VulkanUtil::createIndexBuffer(m_indices, m_index_buffer);
	}

	void StaticMesh::calcBoundingBox()
	{
		for (const auto& vertex : m_vertices)
		{
			m_bounding_box.combine(vertex.m_position);
		}
	}

}