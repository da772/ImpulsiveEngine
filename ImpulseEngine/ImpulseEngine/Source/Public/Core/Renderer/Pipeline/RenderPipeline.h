#pragma once


namespace GEngine {

	class Renderable;
	class FrameBuffer;
	class Shader;

	class RenderPipeline {

	public:

		RenderPipeline();
		RenderPipeline(const char* name);
		virtual ~RenderPipeline() {};

		virtual void Render();
		void Add(Ref<Renderable> r);
		void Remove(Ref<Renderable> r);
		void Sort();
		void Clear();
		void SetSize(const int width, const int height);
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

	protected:
		std::vector<Ref<Renderable>> renderables;
		std::mutex renderMutex;
		std::function<void()> m_shaderEndFunc = nullptr;
		std::function<void()> m_shaderStartFunc = nullptr;
		Ref<FrameBuffer> m_frameBuffer;
		Ref<Shader> m_shader;
		std::string m_name = "";


	};



}