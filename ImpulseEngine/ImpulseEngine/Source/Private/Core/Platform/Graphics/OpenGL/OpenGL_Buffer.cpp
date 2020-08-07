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

namespace GEngine {


//////////////////////////////////////////////////////////////////////////////
//// INDEX BUFFER ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

	OpenGL_VertexBuffer::OpenGL_VertexBuffer(float* vertices, uint32_t size) :
		VertexBuffer(vertices, size/sizeof(uint32_t))
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
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

}

#endif
