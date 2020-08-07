#pragma once
#include "Core/Renderer/Graphics/Shader.h"

#ifdef GE_GRAPHICS_API_VULKAN
#include "vulkan/vulkan.h"
#endif

namespace GEngine {

	class Vulkan_Shader : public Shader {

	public:
		Vulkan_Shader();
		Vulkan_Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files = false);
		Vulkan_Shader(const std::string& filePath);
		~Vulkan_Shader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void UploadUniformIntArray(const std::string& name, int* values, u32 count) override;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) override;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& values)override;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void UploadUniformFloat(const std::string& name, const float values) override;
		virtual void UploadUniformInt(const std::string& name, const int values) override;
		virtual uint32_t ShaderTypeFromString(const std::string& type) override;
		
#ifdef GE_GRAPHICS_API_VULKAN
		static VkPipelineShaderStageCreateInfo CreateShaderModuleFromFile(std::string fileName, uint32_t type);
#endif
	private:
		static std::vector<char> readFileVec(const std::string& fileName);
		void Compile(std::unordered_map<uint32_t, std::vector<char>> shaderSources);
#ifdef GE_GRAPHICS_API_VULKAN
		static VkShaderModule createShaderModule(const std::vector<char>& code);
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		uint32_t m_RendererID = 0;
		std::vector<std::vector<char>> sourceVector;
#endif
		
	};

}
