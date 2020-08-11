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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		 
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

#ifdef GE_GRAPHICS_API_OPENGL_3_3
		gltInit();
#endif

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		glDrawArrays(GL_LINES, 0, vertexArray->GetVertexCount());
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

}


#endif
