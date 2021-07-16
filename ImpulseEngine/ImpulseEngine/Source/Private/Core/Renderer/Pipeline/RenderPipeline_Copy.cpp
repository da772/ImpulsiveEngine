#include "gepch.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline_Copy.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/Renderable.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"

namespace GEngine {

	RenderPipeline_Copy::RenderPipeline_Copy(Ref<RenderPipeline> ref, Camera* cam, const float renderScale /*= 1.f*/, const char* name /*= "Renderpipeline_Copy"*/) : RenderPipeline(name, cam, renderScale, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest), pipeline(ref)
	{

	}

	RenderPipeline_Copy::~RenderPipeline_Copy()
	{

	}

	void RenderPipeline_Copy::RenderStart()
	{
		pipeline->RenderStart();
	}

	void RenderPipeline_Copy::RenderEnd()
	{
		pipeline->RenderEnd();
	}

	void RenderPipeline_Copy::Render()
	{
		std::lock_guard<std::mutex> g(renderMutex);
		m_frameBuffer->Bind();
		Renderer::Prepare();
		RenderStart();
		for (int i = 0; i < pipeline->GetRenderables().size(); i++) {
			pipeline->GetRenderables()[i]->Render(m_camera);
		}
		RenderEnd();
		m_frameBuffer->UnBind();
	}

	void RenderPipeline_Copy::SetCamera(Camera* cam)
	{
		m_camera = cam;
	}

	void RenderPipeline_Copy::SetSize(const int width, const int height)
	{
		RenderPipeline::SetSize(width * Renderer::GetRenderScale(), height * Renderer::GetRenderScale());
	}

}