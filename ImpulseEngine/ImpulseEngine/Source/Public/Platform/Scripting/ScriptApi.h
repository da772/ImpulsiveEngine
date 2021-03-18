#pragma once

#include <reflection/reflection.hpp>


namespace GEngine {

	using NativeObject = refl::uClass;
	using NativeStorage = refl::store::storage*;
	using NativeReflector = refl::reflector*;

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
		static void SetMake_Native(const std::string& dir, const std::string& name);
		static void SetBuild_Native(const std::string& dir, const std::string& name);
		static inline NativeReflector GetReflector_Native() { return s_nativeReflector; }



	private:

		static refl::reflector* s_nativeReflector;
		static std::string dirMake_Native;
		static std::string nameMake_Native;
		static std::string dirBuild_Native;
		static std::string nameBuild_Native;
		static std::shared_ptr<spdlog::logger> native_logger;

	};





}
