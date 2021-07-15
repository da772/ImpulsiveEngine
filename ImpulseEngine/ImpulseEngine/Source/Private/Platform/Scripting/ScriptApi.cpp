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
	std::string ScriptApi::dllDir_Native = "";
	std::function<bool()> ScriptApi::cmdMake_Native = nullptr;

	std::unordered_map<ObjectHash, std::vector<NativePtr>> ScriptApi::nativeScriptReferences;
	std::unordered_set<NativeScriptComponent*> ScriptApi::nativeScripts;
	std::unordered_map<std::string, std::vector<std::string>> ScriptApi::nativeScriptPtrs;

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
		s_nativeReflector->LoadClasses(file.c_str());
	}

	bool ScriptApi::Load(const std::string& path, const std::string& extension)
	{
		return Load_Native(path, extension);
	}

	void ScriptApi::Unload()
	{
		Unload_Native();
	}


	void ScriptApi::SetNativeScriptPtrs(void* hash, void* data)
	{
		refl::uClass* cl = (refl::uClass*)data;
		GameObject* m_go = (GameObject*)hash;
		std::unordered_map<std::string, std::vector<std::string>>::iterator it = nativeScriptPtrs.find(cl->GetName());

		if (it != nativeScriptPtrs.end()) {
			for (const auto& ptr : it->second) {
				GameObject* go = cl->GetMember<GameObject*>(ptr);
				if (go) {
					void** pp = (void**)((uint8_t*)hash + s_nativeReflector->GetStorage()->get_map().at(cl->GetName()).property_map.at(ptr).offset);
					nativeScriptReferences[go->GetHash()].push_back({ m_go->GetHash(), (void**)pp });
				}
			}
		}

	}

	void ScriptApi::RemoveGameObject(ObjectHash hash)
	{
		std::unordered_map<ObjectHash, std::vector<NativePtr>>::iterator it = nativeScriptReferences.find(hash);

		if (it != nativeScriptReferences.end()) {
			for (const auto& i : it->second) {	
				if (GameObject::GetObjectFromHash(i.go)) {
					*i.ptr = nullptr;
				}
			}
			nativeScriptReferences.erase(it);
		}
	
	}

	bool ScriptApi::Load_Native(const std::string& path, const std::string& extension)
	{
#if GE_HOT_RELOAD
		nativeScriptPtrs.clear();
		nativeScriptReferences.clear();
		bool compileFail = false;
		if (lib) {
            s_nativeReflector->Clear();
			Utility::__UnloadLib("NativeScripts", &lib, s_nativeReflector->GetStorage());
		}

#if 1
		for (auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			if (p.path().extension() == extension) {
				Utility::__GenerateLib(p.path().generic_string(), p.path().filename().generic_string(), *s_nativeReflector);
			}
		}
		s_nativeReflector->GenerateClasses();
#endif
#if 1
		uint64_t ms = Time::GetEpochTimeMS();
		if (cmdMake_Native) {
			if (!cmdMake_Native()) {
				GE_CORE_ERROR("CMDMAKE_NATIVE FAILED");
				return false;
			}
		}
		else {
			std::string buildOut = Utility::sys::build_proj(dirMake_Native, nameMake_Native);
			if (buildOut.size() > 0) {
				if (buildOut.find("Done") == std::string::npos) {
					GE_CORE_ERROR("Native Build: Error: {0}", buildOut);
				}
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
					GE_CORE_WARN("Native Compile: Is Debugger Attatched?");
				}
				compileFail = true;
			}
		}
#endif
		ms = Time::GetEpochTimeMS() - ms;
		GE_CORE_TRACE("Native Compile: Complete... ({0}ms)", ms);
		ms = Time::GetEpochTimeMS();
		if (Utility::__LoadLib(dllDir_Native,"NativeScripts", &lib, s_nativeReflector->GetStorage())) {
			

			for (const auto& objs : s_nativeReflector->GetStorage()->get_map()) {
				for (const auto& prop : objs.second.property_map) {
					if (prop.second.type == refl::store::uproperty_type::uclass_ptr) {
						nativeScriptPtrs[objs.first].push_back(prop.first);
					}
				}
			}
			ms = Time::GetEpochTimeMS() - ms;
			GE_CORE_TRACE("Native Link: Complete... ({0}ms)", ms);
		}
		else {
			GE_CORE_ERROR("Native Link: Failed");
			return false;
		}
		
		if (compileFail)
			return false;

		return true;
#else
		return true;
#endif
	}

	void ScriptApi::Unload_Native()
	{
		if (lib) {
			Utility::__UnloadLib("NativeScripts", &lib, s_nativeReflector->GetStorage());
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

	void ScriptApi::SetMake_Native(const std::string& dir, const std::string& name, const std::function<bool()>& f )
	{
		dirMake_Native = dir;
		nameMake_Native = name;
		cmdMake_Native = f;
	}

	void ScriptApi::SetBuild_Native(const std::string& dir, const std::string& name)
	{
		dirBuild_Native = dir;
		nameBuild_Native = name;
	}

	void ScriptApi::SetDLLDir_Native(const std::string& dir)
	{
		dllDir_Native = dir;
	}

	void ScriptApi::SetRelativePath_Native(const std::string& includeDir)
	{
		s_nativeReflector->SetRelativeInclude(includeDir.c_str());
	}



	void ScriptApi::AddNativeScript(NativeScriptComponent* c)
	{
		nativeScripts.insert(c);
	}

	void ScriptApi::RemoveNativeScript(NativeScriptComponent* c)
	{
		nativeScripts.erase(c);
	}

	const std::unordered_set<NativeScriptComponent*>& ScriptApi::GetNativeScripts()
	{
		return nativeScripts;
	}

}
