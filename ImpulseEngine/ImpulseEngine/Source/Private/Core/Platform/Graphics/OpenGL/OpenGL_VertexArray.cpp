#include "gepch.h"
#include "Core/Platform/Graphics/OpenGL/OpenGL_VertexArray.h"

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

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		}

		GE_CORE_ASSERT(false, "Invalid OpenGL ShaderDataType");
		return GL_FALSE;
	}

	OpenGL_VertexArray::OpenGL_VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGL_VertexArray::~OpenGL_VertexArray()
	{
		Bind();
		int count = 0;
		for (int i = 0; i < m_VertexBuffers.size(); i++) {
			for (int j = 0; j < m_VertexBuffers[i]->GetLayout()->GetElements().size(); j++) {
				glDisableVertexAttribArray(count);
				count++;
			}
			
		}
		m_IndexBuffer = nullptr;
		m_VertexBuffers.clear();
		glDeleteVertexArrays(1, &m_RendererID);

	}

	void OpenGL_VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGL_VertexArray::UnBind()
	{
		glBindVertexArray(0);
	}

	void OpenGL_VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) 
	{
		GE_CORE_ASSERT(vertexBuffer->GetLayout()->GetElements().size(), "Vertex Buffer has no layout!");

		Bind();
		vertexBuffer->Bind();
		uint32_t index = 0;
		uint16_t denominator = 0;
		for (const auto& element : *vertexBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
			denominator += element.GetComponentCount();
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, vertexBuffer->GetLayout()->GetStride(),
				reinterpret_cast<void*>((uint64_t)element.Offset));
			index++;
		}

		m_VertexCount += vertexBuffer->GetVertexCount() / denominator;
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGL_VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) 
	{
		Bind();
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
		
	}


}

#endif
