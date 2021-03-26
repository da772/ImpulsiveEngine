#include "gepch.h"
#include "Public/Platform/Scripting/ScriptApi.h"
#include "Public/Core/Util/Time.h"
#include "Public/Core/Util/ThreadPool.h"

namespace GEngine {

	refl::reflector* ScriptApi::s_nativeReflector = nullptr;
	std::string ScriptApi::dirMake_Native = "";
	std::string ScriptApi::nameMake_Native = "";
	std::string ScriptApi::dirBuild_Native = "";
	std::string ScriptApi::nameBuild_Native = "";

	static dllptr lib;

    void ScriptApi::_NativeLog(uint8_t i, const std::string& s) {
        switch (i) {
            case 0: GE_NATIVE_TRACE(s); break;
            case 1: GE_NATIVE_DEBUG(s); break;
            case 2: GE_NATIVE_INFO(s); break;
            case 3: GE_NATIVE_WARN(s); break;
            default:
            case 4: GE_NATIVE_ERROR(s); break;
        }
    }

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
#if GE_HOT_RELOAD
		if (lib) {
            s_nativeReflector->Clear();
			Utility::__UnloadLib("Scripts_CPP", &lib, s_nativeReflector->GetStorage());
		}

#if 1
		for (auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			if (p.path().extension() == extension) {
				Utility::__GenerateLib(path, p.path().stem().string() + ".h", *s_nativeReflector);
			}
		}
#endif
#if 1
		uint64_t ms = Time::GetEpochTimeMS();
		std::string buildOut = Utility::sys::build_proj(dirMake_Native, nameMake_Native);
		if (buildOut.size() > 0) {
			if (buildOut.find("Error:") != std::string::npos) {
				GE_CORE_ERROR("Native Build: Error: {0}", buildOut);
			}
		}
		ms = Time::GetEpochTimeMS() - ms;
		GE_CORE_TRACE("Native Build: Complete... ({0}ms)", ms);
		ms = Time::GetEpochTimeMS();
        std::string compileOut = Utility::sys::compile_proj(dirBuild_Native, nameBuild_Native);
		if (compileOut.size() > 0) {
			if (compileOut.find("error") != std::string::npos) {
				GE_CORE_ERROR("Native Compile: Failure... ");
				GE_CORE_ERROR("{0}", compileOut);
				if (compileOut.find("insufficient") != std::string::npos) {
					GE_CORE_INFO("Native Compile: ");
				}
				return false;
			}
		}
#endif
		ms = Time::GetEpochTimeMS() - ms;
		GE_CORE_TRACE("Native Compile: Complete... ({0}ms)", ms);
		ms = Time::GetEpochTimeMS();
		if (Utility::__LoadLib("Scripts_CPP", &lib, s_nativeReflector->GetStorage())) {
			ms = Time::GetEpochTimeMS() - ms;
			GE_CORE_TRACE("Native Link: Complete... ({0}ms)", ms);
		}
		else {
			GE_CORE_ERROR("Native Link: Failed");
			return false;
		}
		
		return true;
#else
		return true;
#endif
	}

	void ScriptApi::Unload_Native()
	{
		if (lib) {
			Utility::__UnloadLib("Scripts_CPP", &lib, s_nativeReflector->GetStorage());
			s_nativeReflector->Clear();
		}
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

	void ScriptApi::SetMake_Native(const std::string& dir, const std::string& name)
	{
		dirMake_Native = dir;
		nameMake_Native = name;
	}

	void ScriptApi::SetBuild_Native(const std::string& dir, const std::string& name)
	{
		dirBuild_Native = dir;
		nameBuild_Native = name;
	}

}
