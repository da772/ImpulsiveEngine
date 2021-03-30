#pragma once
#include "Core/Renderer/Graphics/Shader.h"


namespace GEngine {

	class GE_API Empty_Shader : public Shader {

	public:
		inline Empty_Shader(){};
		inline Empty_Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files = false){};
		inline Empty_Shader(const std::string& filePath){};
		inline ~Empty_Shader(){};
		inline virtual void Bind() const override{};
		inline virtual void UnBind() const override{};
		inline virtual void Reload() override{};
		inline virtual void Unload() override{};
		inline virtual void UploadUniformIntArray(const std::string& name, int* values, u32 count) override{};
		inline void UploadUniformMat4(const std::string& name,  const glm::mat4& matrix) override{};
		inline void UploadUniformMat3(const std::string& name,  const glm::mat3& matrix) override{};
		inline virtual void UploadUniformFloat4(const std::string& name, const Vector4f& values) override{};
		inline virtual void UploadUniformFloat2(const std::string& name, const Vector2f& values)override{};
		inline virtual void UploadUniformFloat3(const std::string& name, const Vector3f& values) override{};
		inline virtual void UploadUniformFloat(const std::string& name, const float values) override{};
		inline virtual void UploadUniformInt(const std::string& name, const int values) override{};
		inline virtual uint32_t ShaderTypeFromString(const std::string& type) override { return 0; };

	};

}
