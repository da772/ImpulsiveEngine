#pragma once

#include <reflection/reflection.hpp>
#include "Public/Core/Util/Utility.h"
#include "Public/Core/Application/GameObject.h"


namespace GEngine {


    struct NativePtr {
        void** ptr;
    };

	using NativeObject = refl::uClass;
	using NativeStorage = refl::store::storage*;
	using NativeReflector = refl::reflector*;

    class NativeScriptComponent;

	class GE_API ScriptApi {
    private:
        template<typename T>
        static inline std::string toString(const T& t) {
            return std::to_string(t);
        }

        static inline std::string toString(const char* t) {
            return t;
        }

        static inline std::string toString(const std::string& t) {
            return t;
        }
	public:
		static void Initialize();
		static void Shutdown();
		static void Generate_Native(const std::string& file);
		bool static Load(const std::string& path, const std::string& extension);
		static void Unload();
		bool static Load_Native(const std::string& path, const std::string& extension);
		static void Unload_Native();
		static void Clear_Native();
		static void OutputDir_Native(const std::string& s);
		static NativeStorage GetStorage_Native();
		static void SetMake_Native(const std::string& dir, const std::string& name, const std::function<bool()>& command = nullptr);
		static void SetBuild_Native(const std::string& dir, const std::string& name);
        static void SetDLLDir_Native(const std::string& dir);
        static void SetRelativePath_Native(const std::string& includeDir);
		static inline NativeReflector GetReflector_Native() { return s_nativeReflector; }
        static void SetNativeScriptPtrs(void* hash, void* data);
        static void RemoveGameObject(ObjectHash hash, void* ptr);

        template<typename ... Args>
        static inline void NativeLog(uint8_t i, const std::string& __s, Args const &... args) {
            std::string s = __s;
            std::regex reg("[{]\\d[}]");
            std::vector<std::string> result;
            int unpack[]{0, (result.push_back(toString(args)), 0)...};
            static_cast<void>(unpack);
            std::smatch sm;
            while(regex_search(s, sm, reg)) {
                std::string it(sm.str());
                size_t pos = s.find(it);
                s.erase(pos, it.size());
                it.erase(0,1);
                it.erase(it.size()-1,1);
                s.insert(pos, result[std::stoi(it)]);
            }
        
            ScriptApi::_NativeLog(i, s);
        
        };

        static void AddNativeScript(NativeScriptComponent*);
        static void RemoveNativeScript(NativeScriptComponent*);
        static const std::unordered_set<NativeScriptComponent*>& GetNativeScripts() ;
            

	private:
		static refl::reflector* s_nativeReflector;
		static std::string dirMake_Native;
		static std::string nameMake_Native;
        static std::function<bool()> cmdMake_Native;
        static std::string dllDir_Native;
		static std::string dirBuild_Native;
		static std::string nameBuild_Native;
        static void _NativeLog(uint8_t i, const std::string& s);
        static std::unordered_set<NativeScriptComponent*> nativeScripts;
        static std::unordered_map<ObjectHash, std::vector<NativePtr>> nativeScriptReferences;
        static std::unordered_map<std::string, std::vector<std::string>> nativeScriptPtrs;

	};





}
