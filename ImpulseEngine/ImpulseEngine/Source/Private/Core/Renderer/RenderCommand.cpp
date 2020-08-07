#include "gepch.h"

#include "Core/Renderer/RenderCommand.h"

namespace GEngine {


	Scope<RendererApi> RenderCommand::s_RendererAPI;

	void RenderCommand::Destroy()
	{
		RenderCommand::s_RendererAPI->Shutdown(); 
		RenderCommand::s_RendererAPI.reset();
	}



}