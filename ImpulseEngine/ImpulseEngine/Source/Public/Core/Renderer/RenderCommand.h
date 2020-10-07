#pragma once
#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/RendererApi.h"

#include <glm/glm.hpp>

namespace GEngine {
	class RenderCommand {
	public:

		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); };
		inline static void Clear() { s_RendererAPI->Clear(); };

		inline static void Init() { s_RendererAPI->Init(); }

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 size = 0) { s_RendererAPI->DrawIndexed(vertexArray, size); }
		inline static void DrawArrays(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawArrays(vertexArray); }
		inline static void DrawArraysLines(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawArraysLines(vertexArray); }

		inline static void DrawArraysInstanced(const Ref<VertexArray>& vertexArray, int amount) { s_RendererAPI->DrawArraysInstanced(vertexArray, amount); };

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); };

		inline static void DrawText3D(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) {
			RenderCommand::s_RendererAPI->DrawText3D(txt, color, scale, model, view, projection, center);
		};

		


		inline static void DrawText3D_Billboard(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) {
			RenderCommand::s_RendererAPI->DrawText3D_Billboard(txt, color, scale, model, view, projection, center);
		};

		

		inline static int GetMaxTextureSlots() { return RenderCommand::s_RendererAPI->GetMaxTextureSlots(); };

		inline static void SetDepth(bool b) { RenderCommand::s_RendererAPI->SetDepth(b); }

		inline static void BindTexture(int id, int slot) {
			RenderCommand::s_RendererAPI->BindTexture(id, slot);
		};

		inline static void SetCulling(bool cull) { RenderCommand::s_RendererAPI->SetCulling(cull); }

		inline static void ClearStencil() { RenderCommand::s_RendererAPI->ClearStencil(); }
		inline static void StencilParams(int op1, int op2, int op3, int func1, int func2, int func3, int mask) { RenderCommand::s_RendererAPI->SetStencil(op1, op2, op3, func1, func2, func3, mask); }

		inline static void Create() { RenderCommand::s_RendererAPI.reset(RendererApi::Create()); }
		static void Destroy();
	private:
		static Scope<RendererApi> s_RendererAPI;

	};



}
