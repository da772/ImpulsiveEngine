#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_ui.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Application/Application.h"

#include "Public/Core/Renderer/Graphics/Renderable.h"


namespace GEngine {

	RenderPipeline_ui::RenderPipeline_ui() : RenderPipeline("RenderPipeline_ui", nullptr)
	{
		
	}

	void RenderPipeline_ui::RenderStart()
	{
		Renderer::SetDepth(false);	
	}

	void RenderPipeline_ui::RenderEnd()
	{
		Renderer::SetDepth(true);
	}



}