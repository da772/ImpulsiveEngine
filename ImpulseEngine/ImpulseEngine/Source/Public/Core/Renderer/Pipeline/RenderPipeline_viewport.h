#pragma once

#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"


namespace GEngine {

	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;

	class GE_API RenderPipeline_viewport : public RenderPipeline {

	public:
		RenderPipeline_viewport();
		virtual ~RenderPipeline_viewport() {};

		virtual void RenderStart() override;
		virtual void RenderEnd() override;
		virtual void Render() override;
		virtual void Unload() override;
		virtual void Reload() override;

	private:
		void CreateGraphics();

	protected:
		Ref<VertexArray> m_varray;
		Ref<VertexBuffer> m_vb;
		Ref<IndexBuffer> m_ib;

	};



}