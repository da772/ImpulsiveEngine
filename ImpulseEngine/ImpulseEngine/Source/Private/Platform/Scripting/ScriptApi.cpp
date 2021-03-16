#include "gepch.h"
#include "Public/Platform/Scripting/ScriptApi.h"
#include "reflection/utility.hpp"

namespace GEngine {

	refl::reflector* ScriptApi::s_nativeReflector = nullptr;
	static dllptr lib;

	void ScriptApi::Initialize()
	{
		if (ScriptApi::s_nativeReflector) GE_CORE_ASSERT(false, "NATIVE REFLECTOR ALREADY CREATED");
		ScriptApi::s_nativeReflector = new refl::reflector();
	}

	void ScriptApi::Shutdown()
	{
		ScriptApi::s_nativeReflector->Clear();
		delete s_nativeReflector;
	}

	void ScriptApi::Generate_Native(const std::string& file)
	{
		s_nativeReflector->Generate(file.c_str());
	}

	bool ScriptApi::Load(const std::string& path, const std::string& extension)
	{
		return Load_Native(path, extension);
	}

	void ScriptApi::Unload()
	{
		Unload_Native();
	}

	bool ScriptApi::Load_Native(const std::string& path, const std::string& extension)
	{
		if (lib) {
			__UnloadLib(&lib, *s_nativeReflector);
			s_nativeReflector->Clear();
		}

		for (auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			if (p.path().extension() == extension) {
				__GenerateLib(path, p.path().stem().string() + ".h", *s_nativeReflector);
			}
		}
			
		std::string buildOut = sys::build_proj(files::GetParentExecuteableDir(3), "GenerateMake");
		if (buildOut.size() > 0) {
			std::cout << buildOut << std::endl;
		}
		std::cout << "Build Complete...\n" << std::endl;
		std::string compileOut = sys::compile_proj(files::GetParentExecuteableDir(3), "Reflection_Tests_Scripts");
		if (compileOut.size() > 0) {
			if (compileOut.find("error") != std::string::npos) {
				std::cout << compileOut << std::endl;
				if (compileOut.find("insufficient") != std::string::npos) {
					std::cout << "IS DEBUGGER ATTATCHED?" << std::endl;
				}
				return false;
			}
		}
		std::cout << "Compile Complete...\n" << std::endl;
		__LoadLib(&lib, *s_nativeReflector);
		std::cout << "LOAD DLL" << std::endl;
		return true;
	}

	void ScriptApi::Unload_Native()
	{

	}

	void ScriptApi::Clear_Native()
	{
		s_nativeReflector->Clear();
	}

	void ScriptApi::OutputDir_Native(const std::string& s)
	{
		s_nativeReflector->SetOutputDir(s.c_str());
	}

	GEngine::NativeStorage ScriptApi::GetStorage_Native()
	{
		return s_nativeReflector->GetStorage();
	}

}