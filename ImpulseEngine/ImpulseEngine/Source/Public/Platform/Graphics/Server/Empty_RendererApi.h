 #pragma once

#include "Core/Renderer/RendererApi.h"

namespace GEngine {

	class GE_API Empty_RendererApi : public RendererApi {
	public:
		inline virtual void Init() override {};
		inline virtual void Shutdown() override {};
		inline virtual void SetClearColor(const Vector4f& color) override{};
		inline virtual void Clear() override{};
		inline virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override{};
		inline virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 size = 0) override{};
		inline virtual void DrawArrays(const Ref<VertexArray>& vertexArray) override{};
		inline virtual void DrawArraysInstanced(const Ref<VertexArray>& vertexArray, int amount) override{};
		inline virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, u32 size ,int amount) override{};
		inline virtual void DrawArraysLines(const Ref<VertexArray>& vertexArray) override{};
		inline virtual void DrawText3D(const char* txt, const Vector4f& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) override {};
		inline virtual void DrawText3D_Billboard(const char* txt, const Vector4f& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) override {};
		inline virtual void SetCulling(bool cull) override {};
		inline virtual void SetDepth(bool b) override{};
		inline virtual int GetMaxTextureSlots() override { return 1;};
		inline virtual void BindTexture(int id, int slot) override {};
		inline virtual void SetBlendMode(int sFactor, int dFactor) override {};
		inline virtual void ClearStencil() override {};
		inline virtual void SetStencil(int op1, int op2, int op3, int func1, int func2, int func3, int mask) override {};
        inline virtual uint32_t GetDefaultFramebufferId() override { return 0;};
		inline virtual void BlendEquationSeparate(int rgb, int alpha) override {};
		inline virtual void StencilMask(int mask) override {};
		inline virtual void StencilFunc(int p1, int p2, int p3) override {};
		inline virtual void BlendFuncSeparate(int p1, int p2, int p3, int p4) override {};
		inline virtual void BlendFunc(int p1, int p2) override {};
        


	};

}
