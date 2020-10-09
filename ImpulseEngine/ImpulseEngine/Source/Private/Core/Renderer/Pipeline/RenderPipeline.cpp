#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"
#include "Public/Core/Renderer/Graphics/Renderable.h"

#include "Public/Core/Renderer/Renderer2D.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"

#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Renderer.h"


namespace GEngine {


	RenderPipeline::RenderPipeline()
	{
		m_frameBuffer = FrameBuffer::Create(0, 0, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear| TEXTUREFLAGS_Min_Linear, m_name.c_str());
		m_shader = Shader::Create("Content/shaders/ViewportShader.glsl");
	}

	RenderPipeline::RenderPipeline(const char* name)
	{
		m_name = name;
		m_frameBuffer = FrameBuffer::Create(0, 0, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Linear, m_name.c_str());
		m_shader = Shader::Create("Content/shaders/ViewportShader.glsl");
	}

	void RenderPipeline::Render()
	{
		std::lock_guard<std::mutex> g(renderMutex);
		m_frameBuffer->Bind();
		Renderer::Prepare();
		RenderStart();
		for (int i = 0; i < renderables.size(); i++) {
			renderables[i]->Render();
		}
		RenderEnd();
		m_frameBuffer->UnBind();
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
		m_frameBuffer->UpdateSize(width, height);
	}

	void RenderPipeline::Unload()
	{
		m_frameBuffer->Unload();
	}

	void RenderPipeline::Reload()
	{
		m_frameBuffer->Reload();
	}

}