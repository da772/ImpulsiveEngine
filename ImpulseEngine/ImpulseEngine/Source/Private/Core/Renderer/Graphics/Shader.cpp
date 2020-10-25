#include "gepch.h"
#include "Core/Renderer/Graphics/Shader.h"
#include "Core/Renderer/Graphics/GraphicsContext.h" 
#include "Public/Core/Util/Pooling.h"
#include "Public/Core/FileSystem/FileSystem.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Core/Platform/Graphics/OpenGL/OpenGL_Shader.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Core/Platform/Graphics/Vulkan/Vulkan_Shader.h"
#endif


namespace GEngine {


	std::unordered_map<std::string, Weak<Shader>> Shader::s_ShaderPool;
	uint32_t Shader::s_shaderID = 0;

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files) {

		Ref<Shader> s = nullptr;

		if (Shader::s_ShaderPool.find(name) != Shader::s_ShaderPool.end()) {
			s = Shader::s_ShaderPool[name].lock();
			if (s) {
				return s;
			}
			s = nullptr;
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			s = make_unique<OpenGL_Shader>(name, vertexSrc, fragmentSrc, files);
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			s = make_unique<Vulkan_Shader>(name, vertexSrc, fragmentSrc, files);
#endif

		default:
			GE_CORE_ASSERT(false, "Undefined Shader for current Graphics API: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;

		}

		s->m_Name = name;
		Shader::s_ShaderPool[name] = s;
		return s;

	}


	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		Ref<Shader> s = nullptr;

		if (Shader::s_ShaderPool.find(filePath) != Shader::s_ShaderPool.end()) {
			if (Shader::s_ShaderPool[filePath].expired()) {
				Shader::s_ShaderPool.erase(filePath);
			} else 
				return Shader::s_ShaderPool[filePath].lock();
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			s = make_unique<OpenGL_Shader>(filePath);
			break;
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			s = make_unique<Vulkan_Shader>(filePath);
			break;
#endif
		default:
			GE_CORE_ASSERT(false, "Undefined Shader for current Graphics API: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;
		}

		s->m_Name = filePath;
		Shader::s_ShaderPool[filePath] = s;
		return s;

	}

	void Shader::UnloadShaders() {
		
		for (std::pair<std::string, Weak<Shader>> s : s_ShaderPool) {
			s.second.lock()->Unload();
		}
	}

	void Shader::ReloadShaders() {
		for (std::pair<std::string, Weak<Shader>> s : s_ShaderPool) {
			s.second.lock()->Reload();
		}
	}

	void Shader::Destroy(Shader* shader)
	{
		
	}

	std::unordered_map<uint32_t, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<uint32_t, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			GE_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[(uint32_t)ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}

		return shaderSources;
	}

	std::string Shader::ReadFile(const std::string& filePath)
	{
		Ref<FileData> data = GEngine::FileSystem::FileDataFromPath(filePath);
		const char* filestr = (char*)data->GetDataAsString();
		return std::string(filestr);
	}


	void Shader::SetShaderID(const uint32_t id)
	{
		s_shaderID = id;
	}

	const uint32_t Shader::GetShaderID() 
	{
		return s_shaderID;
	}

}
