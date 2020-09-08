#pragma once


namespace GEngine {

	class Renderable;
	class FrameBuffer;

	class RenderPipeline {

	public:

		RenderPipeline() {};
		virtual ~RenderPipeline() {};

		void Render();
		void Add(Ref<Renderable> r);
		void Remove(Ref<Renderable> r);
		void Sort();
		void Clear();
		void SetSize(const int width, const int height);
		static Ref<FrameBuffer> GetFrameBuffer() { return s_frameBuffer; };
		virtual void RenderStart() {};
		virtual void RenderEnd() {};
		virtual void Unload() {};
		virtual void Reload() {};

	protected:
		std::vector<Ref<Renderable>> renderables;
		std::mutex renderMutex;
		static Ref<FrameBuffer> s_frameBuffer;


	};



}