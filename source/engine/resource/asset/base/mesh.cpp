#include "mesh.h"

namespace Lacrima
{

	Mesh::~Mesh()
	{
		m_vertex_buffer.destroy();
		m_index_buffer.destroy();
	}

}