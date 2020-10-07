#pragma once

#include <glm/glm.hpp>
#include "Public/Core/Renderer/Graphics/VertexArray.h"

#define BLEND_SRC_COLOR			         0x0300
#define BLEND_ONE_MINUS_SRC_COLOR        0x0301
#define BLEND_SRC_ALPHA                  0x0302
#define BLEND_ONE_MINUS_SRC_ALPHA        0x0303
#define BLEND_DST_ALPHA		             0x0304
#define BLEND_ONE_MINUS_DST_ALPHA        0x0305
#define BLEND_DST_COLOR                  0x0306
#define BLEND_ONE_MINUS_DST_COLOR        0x0307
#define BLEND_SRC_APLHA_SATURATE         0x0308

#define STENCIL_KEEP 0x1E00
#define STENCIL_REPLACE 0x1E01
#define STENCIL_INCR 0x1E02
#define STENCIL_DECR 0x1E03

#define STENCIL_NEVER 0x0200
#define STENCIL_LESS 0x0201
#define STENCIL_EQUAL 0x0202
#define STENCIL_LEQUAL 0x0203
#define STENCIL_GREATER 0x0204
#define STENCIL_NOTEQUAL 0x0205
#define STENCIL_GEQUAL 0x0206
#define STENCIL_ALWAYS 0x0207






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
		virtual void SetBlendMode(int sFactor, int dFactor) = 0;
		inline virtual void GetBlendMode(int* sFactor, int* dFactor) { *sFactor = m_sFactor; *dFactor = m_dFactor; };
		inline virtual void BindTexture(int id, int slot = 0) {};
		inline virtual void ClearStencil() {};
		inline virtual void SetStencil(int op1, int op2, int op3, int func1, int func2, int func3, int mask) {};
        virtual uint32_t GetDefaultFramebufferId() {};
		virtual void Shutdown() = 0;
		
		static RendererApi* Create();

	protected:
		static unordered_map<int, int> s_BlendFactors;
		int m_sFactor, m_dFactor;

	};



}
