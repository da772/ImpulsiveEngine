#include "gepch.h"
#include "Public/Core/Platform/Graphics/OpenGL/OpenGL_Texture.h"

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

#include "stbImage/stb/stb_image.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/FileSystem/FileSystem.h"
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"

namespace GEngine {

	OpenGL_Texture2D::OpenGL_Texture2D(const std::string& path, const u32 flags)
		: m_Path(path), m_Height(0), m_Width (0)
	{
		m_flags = flags;
		
		if (!ThreadPool::OnMainThread()) {
			ThreadPool::AddMainThreadFunction([this, path]() {
				m_mutex.lock();
				Ref<FileData> _data = GEngine::FileSystem::FileDataFromPath(path);
				UploadDataSTBI(_data->GetData(), _data->GetDataSize()); 
				m_mutex.unlock();
				SubTexture2D::m_condVar.notify_all();
				});
			return;

		}
		Ref<FileData> _data = GEngine::FileSystem::FileDataFromPath(path);
		UploadDataSTBI(_data->GetData(), _data->GetDataSize());		

	}

	OpenGL_Texture2D::OpenGL_Texture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
	}

	void OpenGL_Texture2D::SetData(void* data, uint32_t size, u32 flags)
	{
		m_flags = flags;
		m_data = data;
		m_size = size;
		glGenTextures(1, (unsigned int*)&m_RendererID);
        
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		uint64_t channel = GL_RGBA;

        if (m_flags & TEXTUREFLAGS_RED_CHANNEL) {
#ifdef GE_GRAPHICS_API_OPENGL_3_3
            channel = GL_RED;
#endif
#ifdef GE_GRAPHICS_API_OPENGL_ES
            channel = GL_LUMINANCE;
#endif
        }
		if (m_flags & TEXTUREFLAGS_GREEN_CHANNEL)
			channel = GL_GREEN;
		if (m_flags & TEXTUREFLAGS_BLUE_CHANNEL)
			channel = GL_BLUE;
		if (m_flags & TEXTUREFLAGS_RGB_CHANNEL)
			channel = GL_RGB;
		if (m_flags & TEXTUREFLAGS_RGBA_CHANNEL)
			channel = GL_RGBA;
		if (m_flags & TEXTUREFLAGS_ALPHA_CHANNEL)
			channel = GL_ALPHA;
		

		glTexImage2D(GL_TEXTURE_2D, 0, channel, m_Width, m_Height, 0, channel, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			(m_flags & TEXTUREFLAGS_DisableMipMap) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);

		if (m_flags & TEXTUREFLAGS_Wrap_Repeat) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		if (m_flags & TEXTUREFLAGS_Mag_Linear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Mag_Linear_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Mag_Nearest_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Min_Nearest) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		if (m_flags & TEXTUREFLAGS_Min_Linear_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Min_Nearest_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}

		if (!(m_flags & TEXTUREFLAGS_DisableMipMap)) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}

	}

	void OpenGL_Texture2D::UploadDataSTBI(unsigned char* _data, uint64_t _size)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load_from_memory(_data,_size, &width, &height, &channels, 0);
		//stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);

		GE_CORE_ASSERT(data && _size > 0, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4) {
			internalFormat = GL_RGBA;
			dataFormat = GL_RGBA;

		}
		else if (channels == 3) {
			internalFormat = GL_RGB;
			dataFormat = GL_RGB;
		}
        
		glGenTextures(1, (unsigned int*)&m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, m_Width, m_Height, 0, internalFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			(m_flags & TEXTUREFLAGS_DisableMipMap) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);

		if (m_flags & TEXTUREFLAGS_Wrap_Repeat) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		if (m_flags & TEXTUREFLAGS_Mag_Linear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Mag_Linear_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Mag_Nearest_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}

		if (m_flags & TEXTUREFLAGS_Min_Nearest) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		if (m_flags & TEXTUREFLAGS_Min_Linear_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (m_flags & TEXTUREFLAGS_Min_Nearest_MipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}

		if (!(m_flags & TEXTUREFLAGS_DisableMipMap)) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		stbi_image_free(data);
	}

	OpenGL_Texture2D::~OpenGL_Texture2D()
	{
		glDeleteTextures(1, (unsigned int*)&m_RendererID);
		
	}

	void OpenGL_Texture2D::Bind(uint32_t slot) const
	{
		
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGL_Texture2D::Unload(){
		glDeleteTextures(1,(unsigned int*)&m_RendererID);
		m_RendererID = 0;
	}

	void OpenGL_Texture2D::Reload() {
		if (m_data != nullptr) {
			SetData(m_data, m_size, m_flags);
		} else {
			Ref<FileData> _data = GEngine::FileSystem::FileDataFromPath(m_Path);
			UploadDataSTBI(_data->GetData(), _data->GetDataSize());		
		}
		
	}



}


#endif
