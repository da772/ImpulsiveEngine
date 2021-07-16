#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_2d.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	RenderPipeline_2d::RenderPipeline_2d(const float renderScale, const char* name) : RenderPipeline(name, nullptr, renderScale, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest) {

	}

	RenderPipeline_2d::~RenderPipeline_2d()
	{

	}

	void RenderPipeline_2d::RenderStart()
	{
		Renderer::SetDepth(false);
	}

	void RenderPipeline_2d::RenderEnd()
	{
		Renderer::SetDepth(true);
	}

	void RenderPipeline_2d::SetCamera(Camera* cam)
	{
		m_camera = cam;
	}

	void RenderPipeline_2d::SetSize(const int width, const int height)
	{
		RenderPipeline::SetSize(width * Renderer::GetRenderScale(), height * Renderer::GetRenderScale());
	}

}