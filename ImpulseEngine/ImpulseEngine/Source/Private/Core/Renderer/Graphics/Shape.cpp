#include "gepch.h"
#include "Public/Core/Renderer/Graphics/Shape.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"

namespace GEngine {

	
	u32 Shape::GetVerticesRows()
	{
		return m_Vertices.size()/m_BufferLayout->GetStride();
	}

	u32 Shape::GetVerticesSize()
	{
		return m_Vertices.size();
	}

	glm::u32 Shape::GetVerticesStride()
	{
		return m_BufferLayout->GetStride();
	}

	u32 Shape::GetIndicesSize()
	{
		return m_Indices.size();
	}


	int Shape::GetTextureSlotOffset()
	{
		for (const BufferElement& b : *m_BufferLayout) {
			if (b.DataName == ShaderDataName::TextureSlot) {
				return b.elemOffset;
			}
		}

		return -1;
	}

}