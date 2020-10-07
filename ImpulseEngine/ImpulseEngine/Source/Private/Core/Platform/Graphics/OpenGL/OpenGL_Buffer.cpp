#include "gepch.h"
#include "Core/Platform/Graphics/OpenGL/OpenGL_Buffer.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)



#ifdef GE_GRAPHICS_API_OPENGL_3_3
#include <glad/glad.h>
#endif

#ifdef GE_GRAPHICS_API_OPENGL_ES
#if defined(GE_PLATFORM_ANDROID)
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#else
#include <OpenGLES/ES3/gl.h>
#endif
#endif
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Application/Application.h"

namespace GEngine {


//////////////////////////////////////////////////////////////////////////////
//// INDEX BUFFER ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

	OpenGL_VertexBuffer::OpenGL_VertexBuffer(float* vertices, uint32_t size) :
		VertexBuffer(vertices, size/sizeof(uint32_t))
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		m_VertexCount = size / sizeof(uint32_t);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGL_VertexBuffer::OpenGL_VertexBuffer(uint32_t size) : VertexBuffer(size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGL_VertexBuffer::~OpenGL_VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
		m_Layout = nullptr;
	}

	void OpenGL_VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGL_VertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGL_VertexBuffer::SetVertices(float* vertices, uint32_t size)
	{
		Bind();
		m_VertexCount = size / sizeof(uint32_t);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);
	}

	void OpenGL_VertexBuffer::SetData(const void* data, u32 size)
	{
		Bind();
		m_VertexCount = size / sizeof(uint32_t);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

//////////////////////////////////////////////////////////////////////////////
//// INDEX BUFFER ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


	OpenGL_IndexBuffer::OpenGL_IndexBuffer(uint32_t* indices, uint32_t size)
		: IndexBuffer(indices, size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, GetIndexCount()*sizeof(uint32_t), indices, GL_STATIC_DRAW);

	}

	OpenGL_IndexBuffer::~OpenGL_IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGL_IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGL_IndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
//////////////////////////////////////////////////////////////////////////////
//// FRAME BUFFER ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


	OpenGL_FrameBuffer::OpenGL_FrameBuffer(int width, int height, int format) : FrameBuffer(width, height, format)
	{
		Create();
	}

	OpenGL_FrameBuffer::~OpenGL_FrameBuffer()
	{
		if (m_rendererId != 0)
			glDeleteFramebuffers(1, &m_rendererId);
		m_texture = nullptr;
	}

	void OpenGL_FrameBuffer::Create()
	{
		glGenFramebuffers(1, &m_rendererId);
		Bind();
		m_texture = Texture2D::Create("", m_width, m_height);
		m_texture->SetData(NULL, 0, m_format, m_width, m_height);
		m_texture->Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetRendererID(), 0);
		m_texture->UnBind();
		UnBind();
	}

	void OpenGL_FrameBuffer::Invalidate()
	{
		m_texture = nullptr;
		if (m_rendererId != 0)
			glDeleteFramebuffers(1, &m_rendererId);
		m_rendererId = 0;
		UnBind();
	}

	void OpenGL_FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
		RenderCommand::SetViewport(0, 0, m_width, m_height);
	}

	void OpenGL_FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (Application::GetApp()->GetWindow())
			RenderCommand::SetViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowWidth());
	}

	void OpenGL_FrameBuffer::UpdateSize(int width, int height)
	{
		//Invalidate();
		m_width = width;
		m_height = height;
		m_texture->SetData(nullptr, 0, m_format, m_width, m_height);
		
	}

	void OpenGL_FrameBuffer::Unload()
	{

		Invalidate();
	}

	void OpenGL_FrameBuffer::Reload()
	{
		if (m_rendererId == 0)
			Create();
	}

}

#endif
