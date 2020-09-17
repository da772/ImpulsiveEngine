#include "Public/Core/Renderer/Graphics/Renderable.h"


namespace GEngine {



	class Shape;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;
	class Texture2D;
	class RenderPipeline;
	class SubTexture2D;




	class Batch : public Renderable {


	public:

		Batch(Ref<Shape> shape, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, int maxShapes, int maxTextures);
		~Batch();
		void SetVertices(std::vector<float>& vertices, std::vector<int>& textures, int indexCount);

		void RefreshVertices();

		void Clear();


		inline std::vector<float>& GetVertices() { return m_Vertices; };
		inline std::vector<int>& GetTextures() { return m_TextureIds; };

		virtual void Render() override;

		static inline const int GetBatchCount() { return s_BatchCount; }


	private:
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		Ref<VertexArray> m_VertexArray = nullptr;
		Ref<Shader> m_Shader = nullptr;
		std::vector<float> m_Vertices;
		std::vector<int> m_TextureIds;
		int m_IndexCount = 0;



		static int s_BatchCount;


	};

	struct BatchObjectData {
		glm::vec3 position;
		float rotation; glm::vec2 scale;  glm::vec4 color; 
		Ref<Texture2D> texture;
		glm::vec2 textureScale;
		float alphaChannel;
		std::vector<float> vertices;
		Ref<SubTexture2D> subTexture = nullptr;
		long long time;
		long batchId = -1;
		uint32_t batchPos = 0;
		int textureId = 0;
	};

	enum class ERenderType {
		GAME = 0,
		UI
	};

	class BatchRenderer {
	public:
		BatchRenderer(ERenderType pipeline, Ref<Shape> shape, int maxShapes, Ref<Shader> shader = nullptr);
		~BatchRenderer();

		long AddShape(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, Ref<Texture2D> texture = nullptr, const glm::vec2& textureScale = { 1,1 }, float alphaChannel = 4);
		long AddShape(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, Ref<SubTexture2D> texture, const glm::vec2& textureScale = { 1,1 }, float alphaChannel = 4);
		long AddShape(BatchObjectData& bData);
		void EditShape(long id, glm::vec3 postiion, float rotation, glm::vec2 scale, glm::vec4 color, Ref<Texture2D> texture = nullptr, const glm::vec2& textureScale = { 1,1 }, float alphaChannel = 4);
		void EditShape(long id, glm::vec3 postiion, float rotation, glm::vec2 scale, glm::vec4 color, Ref<SubTexture2D> texture, const glm::vec2& textureScale, float alphaChannel = 4);
		void RemoveShape(long id);

		void SetColor(long id, glm::vec4 color);
		void SetPosition(long id, glm::vec2 position);
		void SetZOrder(long id, float zOrder);
		void SetSubTexture(long id, Ref<SubTexture2D> texture);
		void SetRotation(long id, float rotation);
		void SetScale(long id, glm::vec2 scale);
		void SetTexture(long id, Ref<Texture2D>);


		void UnloadGraphics();
		void ReloadGraphics();

		const Vector3 GetShapePosition(long id);
		const float GetShapeRotation(long id);
		const Vector2 GetShapeScale(long id);

		int UpdateCount(int i);

		void SetSort(bool b);
		void ReCreateBatches();

		inline void SetRenderType(ERenderType type) { m_renderType = type; }

	private:
		ERenderType m_renderType = ERenderType::GAME;
		void Setup();
		void ReCreateShapeVertices(BatchObjectData* data);
		bool bUnloaded = false;
		std::vector<std::pair<uint32_t, BatchObjectData>> m_SortedObjects;
		Ref<Shape> m_Shape = nullptr;
		Ref<Shader> m_Shader = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		Ref<Texture2D> m_BlankTexture = nullptr;
		std::vector<u32> m_Indices;
		int m_MaxShapes = 0;
		int m_MaxVertices = 0;
		int m_MaxIndices = 0;
		int m_MaxTextures = 0;
		std::vector<Ref<Batch>> m_Batches;
		Ref<RenderPipeline> m_Pipeline = nullptr;
		const char* m_PipelineId;

		int m_RefCount = 0;
		bool m_Sort = true;

		int counter = 0;

		

		

	};

}