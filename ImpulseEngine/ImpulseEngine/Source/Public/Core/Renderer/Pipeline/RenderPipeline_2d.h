#pragma once
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"

namespace GEngine {

	class RenderPipeline_2d : public RenderPipeline {

	public:
		RenderPipeline_2d() {};
		virtual ~RenderPipeline_2d() {};

		virtual void RenderStart() override;
		virtual void RenderEnd() override;

	};

}
