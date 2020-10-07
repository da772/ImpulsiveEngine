#pragma once

#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"


namespace GEngine {

	class RenderPipeline_ui : public RenderPipeline {

	public:
		RenderPipeline_ui();
		virtual ~RenderPipeline_ui() {};

		
		
		virtual void RenderStart() override;
		virtual void RenderEnd() override;

	};



}