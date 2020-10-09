#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_2d.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	RenderPipeline_2d::RenderPipeline_2d() : RenderPipeline("RenderPipeline_2d") {

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



}