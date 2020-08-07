#pragma once


namespace GEngine {

	class Renderable;

	class RenderPipeline {

	public:

		RenderPipeline() {};
		virtual ~RenderPipeline() {};

		void Render();

		void Add(Ref<Renderable> r);
		void Remove(Ref<Renderable> r);
		void Sort();
		virtual void RenderStart() {};
		virtual void RenderEnd() {};

	protected:
		std::vector<Ref<Renderable>> renderables;
		std::mutex renderMutex;

	};



}