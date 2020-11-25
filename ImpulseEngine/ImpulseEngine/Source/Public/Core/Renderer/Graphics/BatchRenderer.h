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

		Batch(Ref<Shape> shape, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, int maxShapes, int maxTextures, const std::function<void()>& shaderFunction = nullptr);
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
		std::function<void()> m_shaderFunction;
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
		uint64_t batchId = -1;
		uint32_t batchPos = 0;
		int textureId = 0;
	};

	enum class ERenderType {
		GAME = 0,
		UI
	};

	class BatchRenderer {
	public:
		BatchRenderer(ERenderType pipeline, Ref<Shape> shape, int maxShapes, Ref<Shader> shader = nullptr, const char* pipelineId = nullptr, const std::function<void()>& shaderFunc = nullptr);
		~BatchRenderer();

		const uint64_t AddShape(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, Ref<Texture2D> texture = nullptr, const glm::vec2& textureScale = { 1,1 }, float alphaChannel = 4);
		const uint64_t AddShape(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, Ref<SubTexture2D> texture, const glm::vec2& textureScale = { 1,1 }, float alphaChannel = 4);
		const uint64_t AddShape(BatchObjectData& bData);
		void EditShape(const uint64_t id, glm::vec3 postiion, float rotation, glm::vec2 scale, glm::vec4 color, Ref<Texture2D> texture = nullptr, const glm::vec2& textureScale = { 1,1 }, float alphaChannel = 4);
		void EditShape(const uint64_t id, glm::vec3 postiion, float rotation, glm::vec2 scale, glm::vec4 color, Ref<SubTexture2D> texture, const glm::vec2& textureScale, float alphaChannel = 4);
		void RemoveShape(const uint64_t id);

		void SetColor(const uint64_t id, glm::vec4 color);
		void SetPosition(const uint64_t id, glm::vec2 position);
		void SetZOrder(const uint64_t id, float zOrder);
		void SetSubTexture(const uint64_t id, Ref<SubTexture2D> texture);
		void SetRotation(const uint64_t id, float rotation);
		void SetScale(const uint64_t id, glm::vec2 scale);
		void SetTexture(const uint64_t id, Ref<Texture2D>);
		void SetTextureScale(const uint64_t id, const glm::vec2& scale);


		void UnloadGraphics();
		void ReloadGraphics();

		const Vector3 GetShapePosition(const uint64_t id);
		const Ref<Texture2D> GetShapeTexture(const uint64_t id);
		const float GetShapeRotation(const uint64_t id);
		const Vector2 GetShapeScale(const uint64_t id);

		int UpdateCount(int i);

		void SetSort(bool b);
		void ReCreateBatches();

		inline void SetRenderType(ERenderType type) { m_renderType = type; }

	private:
		ERenderType m_renderType = ERenderType::GAME;
		void Setup();
		void ReCreateShapeVertices(BatchObjectData* data);
		bool bUnloaded = false;
		std::vector<std::pair<uint64_t, BatchObjectData>> m_SortedObjects;
		Ref<Shape> m_Shape = nullptr;
		Ref<Shader> m_Shader = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		Ref<Texture2D> m_BlankTexture = nullptr;
		std::vector<u32> m_Indices;
		int m_MaxShapes = 0;
		std::function<void()> m_shaderfunction;
		int m_MaxVertices = 0;
		int m_MaxIndices = 0;
		int m_MaxTextures = 0;
		std::vector<Ref<Batch>> m_Batches;
		Ref<RenderPipeline> m_Pipeline = nullptr;
		std::string m_PipelineId;

		int m_RefCount = 0;
		bool m_Sort = true;

		

		

	};

}
