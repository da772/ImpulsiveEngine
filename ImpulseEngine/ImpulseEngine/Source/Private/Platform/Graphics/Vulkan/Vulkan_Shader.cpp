#include "gepch.h"
#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Platform/Graphics/Vulkan/Vulkan_Shader.h"
#include "Public/Platform/Graphics/Vulkan/Vulkan_GraphicsContext.h"

namespace GEngine {

	Vulkan_Shader::Vulkan_Shader()
	{
		
	}

	Vulkan_Shader::Vulkan_Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files)
	{
		Vulkan_GraphicsContext::GenShaderId(m_RendererID);
		Compile({ {VK_SHADER_STAGE_VERTEX_BIT, readFileVec(vertexSrc)}, {VK_SHADER_STAGE_FRAGMENT_BIT, readFileVec(fragmentSrc) } });
	}

	Vulkan_Shader::Vulkan_Shader(const std::string& filePath)
	{
		//Compile(PreProcess(readFileVec(filePath)));
	}

	Vulkan_Shader::~Vulkan_Shader()
	{
		Vulkan_GraphicsContext::DestroyShaderId(m_RendererID);
	}

	void Vulkan_Shader::Bind() const
	{
		Vulkan_GraphicsContext::BindShader(m_RendererID);
	}

	void Vulkan_Shader::UnBind() const
	{
		Vulkan_GraphicsContext::UnBindShader();
	}

	void Vulkan_Shader::UploadUniformIntArray(const std::string& name, int* values, u32 count)
	{

	}

	std::vector<char> Vulkan_Shader::readFileVec(const std::string& fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			GE_CORE_ASSERT(false, "Shader: Failed to open shader file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkPipelineShaderStageCreateInfo Vulkan_Shader::CreateShaderModuleFromFile(std::string fileName, uint32_t type)
	{
		std::vector<char> v = readFileVec(fileName);

		VkShaderModule shaderModule = createShaderModule(v);

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = (VkShaderStageFlagBits)type;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";

		return shaderStageInfo;
	}

	void Vulkan_Shader::Compile(std::unordered_map<uint32_t, std::vector<char>> shaderSources)
	{
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStagesInfos;
		int count = 0;
		for (const auto& KeyValue : shaderSources) {
			std::vector<char> v;
			std::copy(KeyValue.second.begin(), KeyValue.second.end(), std::back_inserter(v));
			//VkShaderModule shaderModule = createShaderModule(v);

			VkPipelineShaderStageCreateInfo shaderStageInfo = {};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = (VkShaderStageFlagBits)KeyValue.first;
			//shaderStageInfo.module = shaderModule;
			shaderStageInfo.pName = "main";

			//shaderModules[count] = shaderModule;
			shaderStagesInfos[count++] = shaderStageInfo;
			sourceVector.push_back(v);
		}

		shaderStages = { shaderStagesInfos[0], shaderStagesInfos[1] };
		Vulkan_GraphicsContext::SetShaderInfo(m_RendererID, shaderStages, sourceVector);

	}


	VkShaderModule Vulkan_Shader::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(Vulkan_GraphicsContext::GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			GE_CORE_ASSERT(false, "Vulkan Api: Failed to create shader module");
		}

		return shaderModule;
	}

	uint32_t Vulkan_Shader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") {
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
		if (type == "fragment" || type == "pixel") {
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		GE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}




	void Vulkan_Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		Vulkan_GraphicsContext::UploadShaderMat4(m_RendererID, matrix);
	}

	void Vulkan_Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{

	}

	void Vulkan_Shader::UploadUniformFloat4(const std::string& name, const Vector4f& values)
	{

	}

	void Vulkan_Shader::UploadUniformFloat2(const std::string& name, const Vector2f& values)
	{

	}

	void Vulkan_Shader::UploadUniformFloat3(const std::string& name, const Vector3f& values)
	{

	}

	void Vulkan_Shader::UploadUniformFloat(const std::string& name, const float values)
	{

	}

	void Vulkan_Shader::UploadUniformInt(const std::string& name, const int values)
	{

	}


}

#endif
