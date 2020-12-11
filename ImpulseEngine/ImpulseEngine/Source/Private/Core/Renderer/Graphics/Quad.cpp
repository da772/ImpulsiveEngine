#include "gepch.h"

#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GEngine {


	Ref<BufferLayout> Quad::s_QuadBufferLayout = Ref<BufferLayout>(new BufferLayout({
			{GEngine::ShaderDataName::Position },
			{GEngine::ShaderDataName::Color },
			{GEngine::ShaderDataName::TextureCoord},
			{GEngine::ShaderDataName::TextureSlot},
			{GEngine::ShaderDataName::TextureScale},
			{GEngine::ShaderDataName::AlphaChannel}
		}));

	Quad::Quad() {
		m_BufferLayout = s_QuadBufferLayout;
		m_Indices = {
			0,1,2,2,3,0
		};
	}

	Quad::~Quad()
	{
		m_BufferLayout.reset();
	}

	u32 Quad::GetVerticesRows()
	{
		return 4;
	}

	u32 Quad::GetVerticesSize()
	{
		// AMT * ROWS
		return 52;
	}

	std::vector<u32> Quad::GetIndices(u32 offset) 
	{
		std::vector<u32> ind = std::vector<u32>(m_Indices.size());
		
		for (int i = 0; i < m_Indices.size(); i++) {
			ind[i] = m_Indices[i] + offset;
		}

		return ind;
	}

	std::vector<float> Quad::GetVertices(Vector3f position, float rotation, Vector3f scale /*= Vector3f(1,1,1)*/,
		Vector4f color /*= Vector4f(1,1,1,1)*/, u32 texture /*= 0*/, Vector2f textureScale, const Vector2f* textureCoords, float alphaChannel)
	{
		std::vector<QuadVertex> vert;

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			* glm::rotate(glm::mat4(1.f), glm::radians(rotation), { 0,0,1.f })
			* glm::scale(glm::mat4(1.0), { scale.x, scale.y, 1.f });

		std::vector<Vector4f> quadPos = {
			{-.5f, -.5f, 0, 1.f},
			{.5f, -.5f, 0.f, 1.f},
			{.5f, .5f, 0.f, 1.f},
			{-.5f, .5f, 0.f,1.f}
		};

		Vector4f _p = transform * quadPos[0];
		vert.push_back({ Vector3f(_p.x,_p.y,_p.z),color, (textureCoords != nullptr ? Vector2f(textureCoords[0]) : Vector2f(0.f,0.f)), (float)texture, textureScale,alphaChannel });

		_p = transform * quadPos[1];
		vert.push_back({ Vector3f(_p.x,_p.y,_p.z) , color,  (textureCoords != nullptr ? Vector2f(textureCoords[1]) : Vector2f(1.f,0.f)), (float)texture, textureScale,alphaChannel });

		_p = transform * quadPos[2];
		vert.push_back({ Vector3f(_p.x,_p.y,_p.z), color,  (textureCoords != nullptr ? Vector2f(textureCoords[2]) : Vector2f(1.f,1.f)), (float)texture, textureScale,alphaChannel });

		_p = transform * quadPos[3];
		vert.push_back({ Vector3f(_p.x,_p.y,_p.z), color,  (textureCoords != nullptr ? Vector2f(textureCoords[3]) : Vector2f(0.f,1.f)), (float)texture, textureScale,alphaChannel });

		std::vector<float> _f((float*)&vert[0], (float*)&vert[0]+GetVerticesSize()/sizeof(float)*GetVerticesRows());
		vert.clear();
		return _f;
	}


	void Quad::SetZPosition(std::vector<float>& arr, float zPos)
	{
		for (int i = 0; i < GetVerticesSize(); i += GetVerticesSize()/GetVerticesRows()) {
			arr[i + 2] = zPos;
		}
	}

	void Quad::SetTextureSlot(std::vector<float>& arr, int texture)
	{
		for (int i = 0; i < GetVerticesSize(); i += GetVerticesSize() / GetVerticesRows()) {
			arr[i + 9] = (float)texture;
		}
	}

}