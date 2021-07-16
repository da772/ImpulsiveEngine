#pragma once


namespace GEngine {

	class Renderable;
	class FrameBuffer;
	class Shader;
	class Camera;

	class GE_API RenderPipeline {

	public:

		RenderPipeline();
		RenderPipeline(const char* name, Camera* camera, const float renderScale = 1.f, uint32_t textureFlags = 1161);
		virtual ~RenderPipeline() {};

		virtual void Render();
		void Add(Ref<Renderable> r);
		void Remove(Ref<Renderable> r);
		void Sort();
		virtual void SetCamera(Camera* camera);
		void Clear();
		virtual void SetSize(const int width, const int height);
		inline Ref<FrameBuffer> GetFrameBuffer() { return m_frameBuffer; };
		virtual void RenderStart() {};
		virtual void RenderEnd() {};
		virtual void Unload();
		virtual void Reload();
		inline Ref<Shader> GetViewPortShader() { return m_shader; }
		inline void SetViewPortShader(Ref<Shader> shader) { m_shader = shader; }
		inline void SetShaderStartFunction(std::function<void()> shaderFunc) { m_shaderStartFunc = shaderFunc; }
		inline void SetShaderEndFunction(std::function<void()> shaderFunc) { m_shaderEndFunc = shaderFunc; }
		inline const std::function<void()>& GetShaderStartFunction() { return m_shaderStartFunc; }
		inline const std::function<void()>& GetShaderEndFunction() { return m_shaderEndFunc; }
		inline Camera* GetCamera() const { return m_camera; }
		inline const uint32_t GetWidth() const { return m_width; }
		inline const uint32_t GetHeight () const { return m_height; }
		inline const std::vector<Ref<Renderable>> GetRenderables() const { return renderables; }

		virtual inline const bool IgnoreViewPort() const { return false; }

	protected:
		std::vector<Ref<Renderable>> renderables;
		std::mutex renderMutex;
		std::function<void()> m_shaderEndFunc = nullptr;
		std::function<void()> m_shaderStartFunc = nullptr;
		Ref<FrameBuffer> m_frameBuffer;
		Ref<Shader> m_shader;
		std::string m_name = "";
		float m_renderScale = 1.f;
		Camera* m_camera = nullptr;
		uint32_t m_width = 0, m_height = 0;
		uint32_t m_textureFlags = 1161;//TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Linear;


	};



}