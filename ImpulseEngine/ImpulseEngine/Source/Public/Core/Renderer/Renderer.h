#pragma once

#include <glm/glm.hpp>



namespace GEngine {

	class VertexBuffer;
	class VertexArray;
	class Shader;
	class Texture2D;
	class Shader;
	class Camera;
	class RenderPipeline;
	class FrameBuffer;

	struct FPipeline {
		FPipeline(Ref<RenderPipeline> p, int i) : p(p), i(i) {

		}
		FPipeline() {};
		Ref<RenderPipeline> p;
		int i;

		bool operator ==(const FPipeline& pipeline) const {
			return p == pipeline.p;
		}

		bool operator == (const Ref<RenderPipeline>& p1) const {
			return p1 == p;
		}

		bool operator <(const FPipeline& p1) const {
			return i < p1.i;
		}

	};

	class Renderer {

	public:
		static void BeginScene(Camera* camera);

		static void Init();

		static void Shutdown();

		static void Render();

		static void Prepare();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void AddPipeline(const char* id, Ref<RenderPipeline> p, int priority);

		static Ref<RenderPipeline> GetPipeline(const char* id);

		static const std::vector<FPipeline>& GetPipelines();

		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f), bool debug = false);

		static void SubmitArrays(Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f), bool debug = false);

		static void SubmitArraysLines(Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));

		static void DrawCube(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color);

		static void DrawLine(const glm::vec3& startPos,const glm::vec3& endPos,const glm::vec4& color);
		static void DrawLines(const std::vector<float>& lines, const glm::vec4& color);
		static void DrawDebugLines(const std::vector<float>& lines, const glm::vec4& color);
		static void DrawCircle(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color);
		
		static void DrawText3D(const char* txt, float scale, const glm::vec3& position, const glm::vec4& color, bool center = true);

		static void DrawText3D_Billboard(const char* txt, float scale, const glm::vec3& position, const glm::vec4& color, bool center = true);
		inline static void SetRenderScale(const float f) { renderScale = f; }
		static const float GetRenderScale();
		static int GetMaxTextureSlots();

		static void SetDepth(bool b);

		static void Unload();
		static void Reload();
		

	private:
		static std::vector<FPipeline> queue;
		static float renderScale;
		static std::unordered_map<std::string, FPipeline> queueId;
		static void OnInit();
		static void CreateBuffers();
		static void DestroyBuffers();


		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::vec3 position;

		};

		static SceneData* s_SceneData;

		

	public:
		struct ShapeData {
			float Box[108] = {
				-1.0f,-1.0f,-1.0f, // triangle 1 : begin
				-1.0f,-1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f, // triangle 1 : end
				1.0f, 1.0f,-1.0f, // triangle 2 : begin
				-1.0f,-1.0f,-1.0f,
				-1.0f, 1.0f,-1.0f, // triangle 2 : end
				1.0f,-1.0f, 1.0f,
				-1.0f,-1.0f,-1.0f,
				1.0f,-1.0f,-1.0f,
				1.0f, 1.0f,-1.0f,
				1.0f,-1.0f,-1.0f,
				-1.0f,-1.0f,-1.0f,
				-1.0f,-1.0f,-1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f,-1.0f,
				1.0f,-1.0f, 1.0f,
				-1.0f,-1.0f, 1.0f,
				-1.0f,-1.0f,-1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f,-1.0f, 1.0f,
				1.0f,-1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f,-1.0f,-1.0f,
				1.0f, 1.0f,-1.0f,
				1.0f,-1.0f,-1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f,-1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f,-1.0f,
				-1.0f, 1.0f,-1.0f,
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f,-1.0f,
				-1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				1.0f,-1.0f, 1.0f
			};

			float line[6] = {
				1,1,1,
				0,0,0
			};

			Ref<GEngine::VertexArray> box_vArray;
			Ref<GEngine::VertexBuffer> box_vBuffer;
			Ref<Shader> shape_shader;
			Ref<GEngine::Texture2D> empty_texture;
			Ref<GEngine::VertexArray> line_vArray;

			Ref<GEngine::VertexBuffer> line_vBuffer;

		};
		static Scope<ShapeData> s_ShapeData;
	private:
		
	};




}