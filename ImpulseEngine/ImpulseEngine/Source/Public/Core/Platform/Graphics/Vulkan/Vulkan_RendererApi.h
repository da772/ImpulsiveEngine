#pragma once
#include "Core/Renderer/RendererApi.h"

namespace GEngine {

	class Vulkan_RendererApi : public RendererApi {
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 size = 0) override;
		virtual void DrawArraysInstanced(const Ref<VertexArray>& vertexArray, int amount) override {};
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawArraysLines(const Ref<VertexArray>& vertexArray) override {};
		virtual void DrawText3D(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) override {};
		virtual void DrawText3D_Billboard(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) override {};
		virtual void SetCulling(bool cull) override {};
		virtual void SetDepth(bool b) override {};
		virtual void Shutdown() override {};
		virtual int GetMaxTextureSlots() override { return 0; };
		virtual void SetBlendMode(int sFactor, int dFactor) override {};

	};

}
