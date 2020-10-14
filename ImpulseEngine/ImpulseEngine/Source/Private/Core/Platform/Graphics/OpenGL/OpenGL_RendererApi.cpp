#include "gepch.h"

#include "Core/Platform/Graphics/OpenGL/OpenGL_RendererApi.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef GE_GRAPHICS_API_OPENGL_3_3
#include <glad/glad.h>
#define GLT_IMPLEMENTATION
#include "gltext.h"
#endif


#ifdef GE_GRAPHICS_API_OPENGL_ES
#if defined(GE_PLATFORM_ANDROID)
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#else
#include <OpenGLES/ES3/gl.h>
#endif
#endif



namespace GEngine {

	void OpenGL_RendererApi::Init()
	{
		s_BlendFactors.clear();
		
		s_BlendFactors[BLEND_SRC_COLOR] = GL_SRC_COLOR;
		s_BlendFactors[BLEND_ONE_MINUS_SRC_COLOR] = GL_ONE_MINUS_SRC_COLOR;
		s_BlendFactors[BLEND_SRC_ALPHA] = GL_SRC_ALPHA;
		s_BlendFactors[BLEND_ONE_MINUS_SRC_ALPHA] = GL_ONE_MINUS_SRC_ALPHA;
		s_BlendFactors[BLEND_DST_ALPHA] = GL_DST_ALPHA;
		s_BlendFactors[BLEND_ONE_MINUS_DST_ALPHA] = GL_ONE_MINUS_DST_ALPHA;
		s_BlendFactors[BLEND_DST_COLOR] = GL_DST_COLOR;
		s_BlendFactors[BLEND_ONE_MINUS_DST_COLOR] = GL_ONE_MINUS_DST_COLOR;
		s_BlendFactors[BLEND_SRC_APLHA_SATURATE] = GL_SRC_ALPHA_SATURATE;
		s_BlendFactors[STENCIL_KEEP] = GL_KEEP;
		s_BlendFactors[STENCIL_REPLACE] = GL_REPLACE;
		s_BlendFactors[STENCIL_INCR] = GL_INCR;
		s_BlendFactors[STENCIL_DECR] = GL_DECR;

		s_BlendFactors[STENCIL_NEVER] = GL_NEVER;
		s_BlendFactors[STENCIL_LESS] = GL_LESS;
		s_BlendFactors[STENCIL_EQUAL] = GL_EQUAL;
		s_BlendFactors[STENCIL_LEQUAL] = GL_LEQUAL;
		s_BlendFactors[STENCIL_GREATER] = GL_GREATER;
		s_BlendFactors[STENCIL_NOTEQUAL] = GL_NOTEQUAL;
		s_BlendFactors[STENCIL_GEQUAL] = GL_GEQUAL;
		s_BlendFactors[STENCIL_ALWAYS] = GL_ALWAYS;
	
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		SetBlendMode(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_STENCIL_TEST);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

        
#ifdef GE_PLATFORM_IOS
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_defaultFrameBuffer);
#endif

#ifdef GE_GRAPHICS_API_OPENGL_3_3
		gltInit();
#endif

	}

	void OpenGL_RendererApi::SetBlendMode(int sFactor, int dFactor)
	{
		m_sFactor = sFactor;
		m_dFactor = dFactor;
		glBlendFunc(s_BlendFactors[sFactor], s_BlendFactors[dFactor]);
	}

	void OpenGL_RendererApi::ClearStencil()
	{
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
	}

	void OpenGL_RendererApi::StencilMask(int mask)
	{
		glStencilMask(mask);
	}

	void OpenGL_RendererApi::StencilFunc(int p1, int p2, int p3)
	{
		glStencilFunc(p1, p2, p3);
	}


	void OpenGL_RendererApi::BlendFuncSeparate(int p1, int p2, int p3, int p4)
	{
		glBlendFuncSeparate(p1, p2, p3, p4);
	}

	void OpenGL_RendererApi::BlendFunc(int p1, int p2)
	{
		glBlendFunc(p1, p2);
	}

	void OpenGL_RendererApi::SetStencil(int op1, int op2, int op3, int func1, int func2, int func3, int mask)
	{
		glStencilOp(op1, op2, op3);
		glStencilFunc(func1, func2, func3);
		glStencilMask(mask);
	}

	void OpenGL_RendererApi::Shutdown()
	{
        #ifdef GE_GRAPHICS_API_OPENGL_3_3
		gltTerminate();
        #endif
	}

	void OpenGL_RendererApi::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGL_RendererApi::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
	}

	void OpenGL_RendererApi::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGL_RendererApi::DrawIndexed(const Ref<VertexArray>& vertexArray, u32 size)
	{
		u32 _size = size <= 0 ? vertexArray->GetIndexBuffer()->GetIndexCount() : size;
		glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGL_RendererApi::DrawArrays(const Ref<VertexArray>& vertexArray)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexCount());
	}

	void OpenGL_RendererApi::DrawArraysInstanced(const Ref<VertexArray>& vertexArray, int amount)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertexArray->GetVertexCount(), amount);
	}

	void OpenGL_RendererApi::DrawArraysLines(const Ref<VertexArray>& vertexArray)
	{
		glDrawArrays(GL_LINE_STRIP, 0, vertexArray->GetVertexCount());
	}

	void OpenGL_RendererApi::DrawText3D(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center)
	{
		#ifdef GE_GRAPHICS_API_OPENGL_3_3
		// Creating text
		GLTtext* text = gltCreateText();
		gltSetText(text, txt);

		// Begin text drawing (this for instance calls glUseProgram)
		gltBeginDraw();
		
		// Draw any amount of text between begin and end
		gltColor(color.x, color.y, color.z, color.w);

		glm::mat4 mvp = projection * view * (center ? glm::translate(glm::mat4(1), glm::vec3(gltGetTextWidth(text, scale) * .5f, gltGetTextHeight(text, scale)*.5f, 0)) : glm::mat4(1)) *model;

		gltDrawText(text, (float*)&mvp[0][0] );


		// Finish drawing text
		gltEndDraw();

		// Deleting text
		gltDeleteText(text);

#endif
	}

	void OpenGL_RendererApi::DrawText3D_Billboard(const char* txt, const glm::vec4& color, const float& scale, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, bool center)
	{
#ifdef GE_GRAPHICS_API_OPENGL_3_3
		// Creating text
		GLTtext* text = gltCreateText();
		gltSetText(text, txt);

		// Begin text drawing (this for instance calls glUseProgram)
		gltBeginDraw();

		// Draw any amount of text between begin and end
		gltColor(color.x, color.y, color.z, color.w);

		gltDrawText3D_Billoard(text, color.x, color.y, color.z, scale, (float*)&view[0][0], (float*)&projection[0][0]);

		// Finish drawing text
		gltEndDraw();

		// Deleting text
		gltDeleteText(text);
#endif
        

	}

	void OpenGL_RendererApi::SetCulling(bool cull)
	{
		if (cull) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}

	void OpenGL_RendererApi::SetDepth(bool b)
	{
		if (b)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	int OpenGL_RendererApi::GetMaxTextureSlots()
	{
		int slots = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &slots);
		return slots;
	}

	void OpenGL_RendererApi::BindTexture(int id, int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

    uint32_t OpenGL_RendererApi::GetDefaultFramebufferId() {
        return m_defaultFrameBuffer;
    }




}


#endif
