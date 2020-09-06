#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"
#include "Public/Core/Renderer/Graphics/Renderable.h"

#include "Public/Core/Renderer/Renderer2D.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"

namespace GEngine {

	Ref<FrameBuffer> RenderPipeline::s_frameBuffer;

	void RenderPipeline::Render()
	{
		std::lock_guard<std::mutex> g(renderMutex);
		RenderStart();
		for (int i = 0; i < renderables.size(); i++) {
			renderables[i]->Render();
		}
		RenderEnd();
	}

	void RenderPipeline::Add(Ref<Renderable> r)
	{
		std::lock_guard<std::mutex> g(renderMutex);
#ifdef GE_DEBUG
		std::vector<Ref<Renderable>>::iterator it = std::find(renderables.begin(), renderables.end(), r);
		GE_CORE_ASSERT(it == renderables.end(), "RENDERABLE ALREADY ADDED");
#endif
		renderables.push_back(r);
		Sort();
	}

	void RenderPipeline::Remove(Ref<Renderable> r)
	{
		std::lock_guard<std::mutex> g(renderMutex);
		std::vector<Ref<Renderable>>::iterator it = std::find(renderables.begin(), renderables.end(), r);
		GE_CORE_ASSERT(it != renderables.end(), "RENDERABLE NOT FOUND");
		if (it != renderables.end()) {
			renderables.erase(it);
		}
	}

	void RenderPipeline::Sort()
	{
		std::sort(renderables.begin(), renderables.end(), [](const Ref<Renderable>& r1, const Ref<Renderable>& r2) {
			return r1->GetPriority() == r2->GetPriority() ? r1->GetTime() < r2->GetTime() :  r1->GetPriority() < r2->GetPriority();
			});
	}

	void RenderPipeline::Clear() {
		renderables.clear();
	}

	void RenderPipeline::SetSize(const int width, const int height)
	{
		s_frameBuffer->UpdateSize(width, height);
	}

}