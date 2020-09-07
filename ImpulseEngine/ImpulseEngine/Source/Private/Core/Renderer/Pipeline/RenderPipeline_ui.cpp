#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_ui.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Application/Application.h"

namespace GEngine {

	RenderPipeline_ui::RenderPipeline_ui()
	{
		if (!s_frameBuffer)
			s_frameBuffer = FrameBuffer::Create(0, 0, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Linear);
	}

	void RenderPipeline_ui::RenderStart()
	{
		Renderer::SetDepth(false);
#if defined( GE_CONSOLE_APP)
		if (Application::DebugTools())
			s_frameBuffer->Bind();
#endif
	}

	void RenderPipeline_ui::RenderEnd()
	{
#if defined( GE_CONSOLE_APP)
		if (Application::DebugTools())
			s_frameBuffer->UnBind();
#endif
		Renderer::SetDepth(true);
	}

}