#pragma once
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"


namespace GEngine {

	class Camera;

	class GE_API RenderPipeline_Copy : public RenderPipeline {

	public:
		RenderPipeline_Copy(Ref<RenderPipeline> pipeline, Camera* cam, const float renderScale = 1.f, const char* name = "Renderpipeline_Copy");
		virtual ~RenderPipeline_Copy();

		virtual void RenderStart() override;
		virtual void RenderEnd() override;

		virtual void Render() override;

		virtual void SetCamera(Camera* cam) override;
		

		virtual inline const bool IgnoreViewPort() const override { return true; } 
		void SetSize(const int width, const int height) override;

	protected:
		Ref<RenderPipeline> pipeline;
		Camera* copyCamera = nullptr;

		bool m_ignoreViewPort = true;

	};

}

