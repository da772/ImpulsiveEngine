#include "gepch.h"

#include "Core/Renderer/RenderCommand.h"

namespace GEngine {

    RendererApi* RenderCommand::s_RendererAPI = nullptr;

	void RenderCommand::Destroy()
	{
		RenderCommand::s_RendererAPI->Shutdown();
        RenderCommand::s_RendererAPI = nullptr;
	}



}
