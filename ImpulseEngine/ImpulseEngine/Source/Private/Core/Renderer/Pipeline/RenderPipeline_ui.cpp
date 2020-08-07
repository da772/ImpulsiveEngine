#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_ui.h"
#include "Public/Core/Renderer/Renderer.h"

namespace GEngine {

	void RenderPipeline_ui::RenderStart()
	{
		Renderer::SetDepth(false);
	}

	void RenderPipeline_ui::RenderEnd()
	{
		Renderer::SetDepth(true);
	}

}