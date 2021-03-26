#pragma once
#include "Core/Renderer/Graphics/Shader.h"


namespace GEngine {

	class GE_API OpenGL_Shader : public Shader {

	public:
		OpenGL_Shader();
		OpenGL_Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files = false);
		OpenGL_Shader(const std::string& filePath);
		~OpenGL_Shader();
	
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void Reload() override;
		virtual void Unload() override;

		virtual void UploadUniformIntArray(const std::string& name, int* values, u32 count) override;
		void UploadUniformMat4(const std::string& name,  const glm::mat4& matrix) override;
		void UploadUniformMat3(const std::string& name,  const glm::mat3& matrix) override;
		virtual void UploadUniformFloat4(const std::string& name, const Vector4f& values) override;
		virtual void UploadUniformFloat2(const std::string& name, const Vector2f& values)override;
		virtual void UploadUniformFloat3(const std::string& name, const Vector3f& values) override;
		virtual void UploadUniformFloat(const std::string& name, const float values) override;
		virtual void UploadUniformInt(const std::string& name, const int values) override;
		virtual uint32_t ShaderTypeFromString(const std::string& type) override;

	private:
		void Compile(std::unordered_map<uint32_t, std::string> shaderSources);
		int m_RendererID = 0;

	};

}
