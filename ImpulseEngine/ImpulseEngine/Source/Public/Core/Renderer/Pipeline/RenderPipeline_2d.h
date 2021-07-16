#pragma once
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"

namespace GEngine {

	class GE_API RenderPipeline_2d : public RenderPipeline {

	public:
		RenderPipeline_2d(const float renderScale = 1.f, const char* name = "Renderpipeline_2d");
		virtual ~RenderPipeline_2d();

		virtual void RenderStart() override;
		virtual void RenderEnd() override;
		virtual void SetCamera(Camera* cam) override;


		void SetSize(const int width, const int height) override;

	};

}
