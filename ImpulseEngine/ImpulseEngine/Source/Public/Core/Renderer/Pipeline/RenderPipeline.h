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
		inline void SetShaderFunction(std::function<void()> shaderFunc) { m_shaderFunc = shaderFunc; }
		inline const std::function<void()>& GetShaderFunction() { return m_shaderFunc; }

	protected:
		std::vector<Ref<Renderable>> renderables;
		std::mutex renderMutex;
		std::function<void()> m_shaderFunc = nullptr;
		Ref<FrameBuffer> m_frameBuffer;
		Ref<Shader> m_shader;
		std::string m_name = "";


	};



}