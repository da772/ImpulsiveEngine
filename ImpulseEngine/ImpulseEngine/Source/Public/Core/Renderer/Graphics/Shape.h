#pragma once

namespace GEngine {

	class BufferLayout;

	class Shape {
	public:
		inline Shape() {};
		inline virtual ~Shape() {};

		virtual u32 GetVerticesRows();
		virtual u32 GetVerticesSize();
		virtual u32 GetVerticesStride();
		virtual u32 GetIndicesSize();
		virtual int GetTextureSlotOffset();
		inline virtual Ref<BufferLayout> GetBufferLayout() { return m_BufferLayout; }
		virtual std::vector<u32> GetIndices(u32 offset) = 0;
		virtual std::vector<float> GetVertices(Vector3 position, float rotation = 0.f, Vector3 scale = Vector3(1, 1, 1), Vector4 color = Vector4(1, 1, 1, 1),
			u32 texture = 0, float textureScale = 1, const Vector2* textureCoords = nullptr, float alphaChannel = 4) = 0;
		virtual void SetZPosition(std::vector<float>& arr, float zPos) = 0;
		virtual void SetTextureSlot(std::vector<float>& arr, int texture) = 0;


	protected:
		std::vector<u32> m_Indices;
		std::vector<float> m_Vertices;
		Ref<BufferLayout> m_BufferLayout;

	};



}
