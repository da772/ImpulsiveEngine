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
		m_frameBuffer = FrameBuffer::Create(0, 0, m_textureFlags, m_name.c_str(), m_renderScale);
		m_shader = Shader::Create("EngineContent/shaders/ViewportShader.glsl");
	}

	RenderPipeline::RenderPipeline(const char* name, Camera* camera, const float renderScale, uint32_t textureFlags) : m_camera(camera)
	{
		m_name = name;
		m_renderScale = renderScale;
		m_textureFlags = textureFlags;
		m_frameBuffer = FrameBuffer::Create(0, 0, m_textureFlags, m_name.c_str(), m_renderScale);
		m_shader = Shader::Create("EngineContent/shaders/ViewportShader.glsl");
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
		r->SetPipeline(this);
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
		m_width = width;
		m_height = height;
		m_frameBuffer->UpdateSize(width*m_renderScale, height * m_renderScale);
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