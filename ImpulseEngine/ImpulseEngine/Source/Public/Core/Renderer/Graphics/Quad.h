#pragma once
#include "Public/Core/Renderer/Graphics/Shape.h"


namespace GEngine {

	struct QuadVertex {
		Vector3 position;
		Vector4 color;
		Vector2 texcoord;
		float texture;
		float textureScale;
		float alphaChannel;
	};

	class Quad : public Shape {
	public:
		Quad();
		virtual ~Quad();

		virtual u32 GetVerticesRows() override;
		virtual u32 GetVerticesSize() override;

		virtual std::vector< u32 > GetIndices(u32 offset) override;
		virtual std::vector<float> GetVertices(Vector3 position, float rotation = 0.f, Vector3 scale = Vector3(1, 1, 1), 
			Vector4 color = Vector4(1, 1, 1, 1), u32 texture = 0, float textureScale = 1,const Vector2* textureCoords = nullptr, float alphaChannel = 4) override;
		virtual void SetZPosition(std::vector<float>& arr, float zPos);
		virtual void SetTextureSlot(std::vector<float>& arr, int texture);
	

	private:



	};


}


