#include "gepch.h"
#include "Core/Platform/Graphics/OpenGL/OpenGL_Shader.h"
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
#include <glm/gtc/type_ptr.hpp>

namespace GEngine {


	uint32_t OpenGL_Shader::ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "pixel") {
			return GL_FRAGMENT_SHADER;
		}

		GE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGL_Shader::OpenGL_Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files)
	{
		m_Name = name;
		Compile({ {GL_VERTEX_SHADER, files ? ReadFile(vertexSrc) : vertexSrc}, {GL_FRAGMENT_SHADER, files ? ReadFile(fragmentSrc) : fragmentSrc } });
	}

	void OpenGL_Shader::Unload() {
		glDeleteProgram(m_RendererID);
		m_RendererID = 0;
	}

	void OpenGL_Shader::Reload() {
		#ifdef GE_GRAPHICS_API_OPENGL_3_3
		Compile(PreProcess(ReadFile(m_Name)));
		#endif
		#ifdef GE_GRAPHICS_API_OPENGL_ES
        std::size_t pos = m_Name.find(".glsl");
        if (pos != std::string::npos) {
            std::string s = m_Name;
            s.replace(pos, 5, "_Mobile.glsl");
            Compile(PreProcess(ReadFile(s)));
            return;
        }
        Compile(PreProcess(ReadFile(m_Name)));
		#endif
	}

	OpenGL_Shader::OpenGL_Shader(const std::string& filePath)
	{
		m_Name = filePath;
#ifdef GE_GRAPHICS_API_OPENGL_3_3
		Compile(PreProcess(ReadFile(filePath)));
#endif
#ifdef GE_GRAPHICS_API_OPENGL_ES
        std::size_t pos = filePath.find(".glsl");
        if (pos != std::string::npos) {
            std::string s = filePath;
            s.replace(pos, 5, "_Mobile.glsl");
            Compile(PreProcess(ReadFile(s)));
            return;
        }
        Compile(PreProcess(ReadFile(filePath)));
#endif
	}


	void OpenGL_Shader::Compile(std::unordered_map<uint32_t, std::string> shaderSources)
	{
		
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;
		GE_CORE_ASSERT(shaderSources.size() <= 2, "Only support 2 shaders per file");
		std::array<GLint, 2> glShaderId;
		int count = 0;
		for (auto& KeyValue : shaderSources) {
			GLenum type = KeyValue.first;
			const std::string& shSource = KeyValue.second;

			GLuint shader = glCreateShader(type);

			const GLchar *source = (const GLchar *)shSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				GE_CORE_ERROR("{0}", infoLog.data());
				GE_CORE_ASSERT(false, "OpenGL: {0} Shader compilation failure!", type);

				return;
			}
			glAttachShader(program, shader);
			glShaderId[count++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (GLuint id : glShaderId) {
				glDeleteShader(id);
			}


			GE_CORE_ERROR("{0}", infoLog.data());
			GE_CORE_ASSERT(false, "OpenGL: Shader Link compilation failure!");
			return;
		}
		for (GLuint id : glShaderId) {
			glDetachShader(program, id);
		}

		m_RendererID = program;

	}


	OpenGL_Shader::~OpenGL_Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGL_Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGL_Shader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGL_Shader::UploadUniformIntArray(const std::string& name, int* values, u32 count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGL_Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGL_Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGL_Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x,values.y,values.z, values.w);
	}

	void OpenGL_Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGL_Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGL_Shader::UploadUniformFloat(const std::string& name, const float values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, values);
	}

	void OpenGL_Shader::UploadUniformInt(const std::string& name, const int values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, values);
	}

	}

#endif
