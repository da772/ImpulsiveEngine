#include "Lighting/Lighting.hpp"

#include "Lighting/RenderPipeline_Lighting.h"

Ref<RenderPipeline> Lighting::m_pipeline;

void Lighting::Initialize()
{
	m_pipeline = std::make_shared<RenderPipeline_Lighting>(Renderer::GetRenderScale());
	Renderer::AddPipeline("lighting", m_pipeline, 499);
}
