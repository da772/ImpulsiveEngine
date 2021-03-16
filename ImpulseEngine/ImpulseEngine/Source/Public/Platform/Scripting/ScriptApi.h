#pragma once

#include <reflection/reflection.hpp>


namespace GEngine {

	using NativeObject = refl::uClass;
	using NativeStorage = refl::store::storage*;

	class ScriptApi {
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

		template<typename ... Args>
		inline static NativeObject CreateObject_Native(const std::string& clazz, Args&& ... args) {
			return s_nativeReflector->CreateUClass(clazz, std::forward<Args>(args)...);
		}

		inline static void DestroyObject_Native(NativeObject& object) {
			s_nativeReflector->DestroyUClass(object);
		}

	private:

		static refl::reflector* s_nativeReflector;

	};





}
