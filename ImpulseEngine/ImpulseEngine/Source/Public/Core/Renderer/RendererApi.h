#pragma once

#include <glm/glm.hpp>
#include "Public/Core/Renderer/Graphics/VertexArray.h"

namespace GEngine {

	class RendererApi {
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 size = 0) = 0;
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawArraysInstanced(const Ref<VertexArray>& vertexArray, int amount) = 0;
		virtual void DrawArraysLines(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawText3D(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) = 0;
		virtual void DrawText3D_Billboard(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center) = 0;
		virtual void SetCulling(bool cull) = 0;
		virtual void SetDepth(bool b) = 0;
		virtual int GetMaxTextureSlots() = 0;

		inline virtual void BindTexture(int id, int slot = 0) {};

		virtual void Shutdown() = 0;
		
		static RendererApi* Create();

	};



}