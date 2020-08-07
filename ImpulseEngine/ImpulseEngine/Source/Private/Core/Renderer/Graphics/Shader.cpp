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


	Scope<ObjectPool<Shader, std::string>> Shader::ShaderPool =
		std::make_unique<ObjectPool<Shader, std::string>>();

	Shader* Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const bool files) {


		Shader* s = nullptr;

		if (Shader::ShaderPool->Contains(name)) {
			return Shader::ShaderPool->Get(name);
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			s = new OpenGL_Shader(name, vertexSrc, fragmentSrc, files);
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			s = new Vulkan_Shader(name, vertexSrc, fragmentSrc, files);
#endif

		default:
			GE_CORE_ASSERT(false, "Undefined Shader for current Graphics API: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;

		}

		s->m_Name = name;
		Shader::ShaderPool->Add(s, s->GetName());
		return s;

	}


	Shader* Shader::Create(const std::string& filePath)
	{
		Shader* s = nullptr;

		if (Shader::ShaderPool->Contains(filePath)) {
			//return Shader::ShaderPool->Get(filePath);
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			s = new OpenGL_Shader(filePath);
			break;
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			s = new Vulkan_Shader(filePath);
			break;
#endif
		default:
			GE_CORE_ASSERT(false, "Undefined Shader for current Graphics API: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;
		}

		s->m_Name = filePath;
		Shader::ShaderPool->Add(s, s->GetName());
		return s;

	}

	void Shader::Destroy(Shader* shader)
	{
		if (Shader::ShaderPool->Remove(shader->GetName())) {
			shader = nullptr;
		}
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


}
