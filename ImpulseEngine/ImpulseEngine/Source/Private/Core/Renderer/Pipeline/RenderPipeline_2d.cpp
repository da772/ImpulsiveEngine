#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_2d.h"
#include "Public/Core/Renderer/Renderer.h"

namespace GEngine {

	void RenderPipeline_2d::RenderStart()
	{
		Renderer::SetDepth(false);
	}

	void RenderPipeline_2d::RenderEnd()
	{
		Renderer::SetDepth(true);
	}

}