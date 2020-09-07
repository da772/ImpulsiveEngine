#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_2d.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	RenderPipeline_2d::RenderPipeline_2d()
	{
#if defined( GE_CONSOLE_APP)
		if (!s_frameBuffer && Application::DebugTools())
			s_frameBuffer = FrameBuffer::Create(Application::GetWidth(), Application::GetHeight(), TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Linear);
#endif
	}

	RenderPipeline_2d::~RenderPipeline_2d()
	{

	}

	void RenderPipeline_2d::RenderStart()
	{
		Renderer::SetDepth(false);
#if defined( GE_CONSOLE_APP)
		if (Application::DebugTools())
			s_frameBuffer->Bind();
#endif
	}

	void RenderPipeline_2d::RenderEnd()
	{
#if defined( GE_CONSOLE_APP) 
		if (Application::DebugTools())
			s_frameBuffer->UnBind();
#endif
		Renderer::SetDepth(true);
	}

}