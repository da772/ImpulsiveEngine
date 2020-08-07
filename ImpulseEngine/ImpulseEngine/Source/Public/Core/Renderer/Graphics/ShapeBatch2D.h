#pragma once

#include "Public/Core/Renderer/Graphics/Shape.h"
#include "Public/Core/Renderer/Graphics/Renderable.h"

namespace GEngine {


	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;
	class Texture2D;
	class SubTexture2D;
	class Shader;

	struct TextureData {
		Ref<Texture2D> texture;
		int refCount = 0;
	};

	struct VertexData {
		Vector3 pos;
		Vector3 scale;
		float rot;
		Vector4 color;
		float texture;
		float textureScale;
		Ref<Texture2D> _texture;
		Ref<SubTexture2D> _subTexture;
		std::vector<Vector2> textCoords;
		std::vector<float> vertices;
		float alphaChannel;
	};

	class BatchData : public Renderable {
	public:
		BatchData(u32 maxShapes, u32 maxTextures, Ref<Shape> shape, Ref<Shader> shader);
		virtual ~BatchData();

		long AddShape(Vector3 position, float rotation = 0, Vector3 scale = Vector3(1,1,1), Vector4 color = Vector4(1,1,1,1), Ref<Texture2D> texture = nullptr, float textureScale = 1, float alphaChannel = 4);
		long AddShape(Vector3 position, float rotation, Vector3 scale, Vector4 color, Ref<SubTexture2D> texture, float textureScale, float alphaChannel = 4);
		void EditShape(long id, Vector3 position, float rotation = 0, Vector3 scale = Vector3(1, 1, 1), Vector4 color = Vector4(1, 1, 1, 1));
		bool RemoveShape(int _id);
		void Clear();
		inline void SetBatchId(int id) { m_id = id; };
		inline int GetBatchId() { return m_id; }
		virtual void Render() override;
		int FindTextureSlot(Ref<Texture2D> texture);
		int AddTexture(Ref<Texture2D> texture);
		void RemoveTextureSlot(u32 id);
		void ReCalculateVertices();
		inline int GetZOrder() { return m_zOrder; };

		void SetShapeRotation(u32 id, float rotation);
		void SetShapeScale(u32 id, Vector3 scale);
		void SetShapeColor(u32 id, Vector4 color);
		void SetShapeTextureScale(u32 id, float scale);

		VertexData& GetVertexData(u32 id);

		/*May Change batch shape id!!*/
		bool SetShapePosition(u32 id, Vector3 position);
		bool SetShapeTexture(u32 id, Ref<Texture2D> texture);
		bool SetShapeSubtexture(u32 id, Ref<SubTexture2D> texture);

	private:
		u32 m_MaxShapes = 10000;
		int m_zOrder = 0;
		u32 m_MaxIndices = 0;
		u32 m_MaxVertices = 0;
		u32 m_MaxTextures = 0;
		Ref<Shape> m_Shape = nullptr;
		std::vector<std::pair<u32, VertexData>> m_SortedVertices;
		std::vector<float> m_Vertices;
		std::vector<u32> m_Indices;
		std::queue<u32> m_VertexPositions;
		std::queue<u32> m_TexturePositions;
		u32 m_VertexCount = 0;
		u32 m_IndexCount = 0;
		u32 m_TextureCount = 0;
		int m_id = 0;
		Ref<VertexArray> m_VertexArray = nullptr;;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		
		std::unordered_map<u32, TextureData> m_Textures;

		Ref<Shader> m_Shader;


	};

	class ShapeBatch2D {
	public:
		ShapeBatch2D(std::string pipelineId, Ref<Shape> shape, Ref<Shader> shader, u32 maxShapes = 10000);
		virtual ~ShapeBatch2D();

		u32 AddShape(Vector3 position, float rotation = 0, Vector3 scale = Vector3(1, 1, 1), Vector4 color = Vector4(1, 1, 1, 1), Ref<Texture2D> texture = nullptr, 
			float textureScale = 1, float alphaChannel = 4);
		u32 AddShape(Vector3 position, float rotation, Vector3 scale, Vector4 color, Ref<SubTexture2D> texture, float textureScale, float alphaChannel = 4);
		void SetShapeRotation(u32 id, float rotation);
		void SetShapeScale(u32 id, Vector3 scale);
		void SetShapeColor(u32 id, Vector4 color);
		void SetShapeTextureScale(u32 id, float scale);

		void EditShape(long id, Vector3 position, float rotation = 0, Vector3 scale = Vector3(1, 1, 1), Vector4 color = Vector4(1, 1, 1, 1));

		/*May Change batch shape id!!*/
		void SetShapePosition(u32 id, Vector3 position);
		void SetShapeTexture(u32 id, Ref<Texture2D> texture);
		void SetShapeSubtexture(u32 id, Ref<SubTexture2D> texture);
		
		/**/
		bool RemoveShape(int id);
		
		int UpdateCount(int i);

	protected:
		void Clear();
		std::pair<u32, u32> ResetAddShape(Vector3 position, float rotation = 0, Vector3 scale = Vector3(1, 1, 1), Vector4 color = Vector4(1, 1, 1, 1), Ref<Texture2D> texture = nullptr, 
			float textureScale = 1, float alphaChannel = 4);
		std::pair<u32,u32> ResetAddShape(Vector3 position, float rotation, Vector3 scale, Vector4 color, Ref<SubTexture2D> texture, float textureScale, float alphaChannel = 4);
		Ref<BatchData> GetBatchFromId(u32 id);
		u32 GetBatchShapeId(u32 id);
		u32 GetBatchIdFromShapeId(u32 id);
		void RemoveBatch(Ref<BatchData> batch);
		u32 AssignShapeID(u32 batchShapeId);
		void RemoveShapeID(u32 id);
		Ref<Texture2D> m_BlankTexture;
		Ref<BatchData> CreateBatch(int priority);
		Ref<Shape> m_Shape;
		std::string m_PipelineId;
		std::queue<u32> m_BatchIds;
		std::unordered_map<u32, u32> m_ShapeIds;
		std::queue<u32> m_ShapeIdPos;
		Ref<Shader> m_Shader = nullptr;
		std::unordered_map<u32, Ref<BatchData>> m_Batches;
		u32 m_MaxShapes = 10000;
		int  m_MaxTextures = -1;
		int refCount = 0;


	};


}
