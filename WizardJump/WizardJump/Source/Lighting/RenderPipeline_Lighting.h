#pragma once

#include <GEngine.h>
using namespace GEngine;

class RenderPipeline_Lighting : public GEngine::RenderPipeline {

public:

	RenderPipeline_Lighting();
	virtual ~RenderPipeline_Lighting();

	void Render() override;


	void RenderStart() override;


	void RenderEnd() override;

	void Unload() override;
	void Reload() override;

private:
	Ref<VertexArray> m_varray;
	Ref<VertexBuffer> m_vb;
	Ref<IndexBuffer> m_ib;
	Ref<FrameBuffer> m_frameBuffer_shadow;
	Ref<FrameBuffer> m_frameBuffer_lights;
	Ref<Shader> m_shaderMask;
	Ref<Shader> m_shaderColor;
	void CreateGraphics();

};