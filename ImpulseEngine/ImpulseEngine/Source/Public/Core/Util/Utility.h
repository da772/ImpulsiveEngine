#pragma once

#include "Public/Core/FileSystem/FileSystem.h"
#include <reflection/reflection.hpp>

#ifdef GE_HOT_RELOAD
#if defined(_WIN32)
using dllptr = HMODULE;
using addrptr = FARPROC;
using ret_err = DWORD;
#endif

#if defined(__APPLE__)
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#endif

#if defined(__linux__) 
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#endif
#else
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#endif

namespace GEngine {

	namespace Utility {
	
		std::string IPV4ToString(uint32_t ip);
		Vector3f ScreenPosToWorldPos(Vector2f pos);
		std::string GenerateHash(const int len);
		void GenerateHash(char* str, const int len);
		namespace sys {

			std::string exec_command(const std::string& cmd);
			std::string build_proj(const std::string& dir, const std::string& file);
			std::string compile_proj(const std::string& dir, const std::string& file);
#ifdef _WIN32
			PROCESS_INFORMATION CreateChildProcess(const std::string& cmd, HANDLE _ERR_WR, HANDLE _OUT_WR);
			std::string ReadFromPipe(HANDLE err_RD, HANDLE out_RD);
#endif

		}
		namespace dll {

			void movePDB(const std::string& dir, const std::string& name, const std::string& newPrefix, bool deleteOld);

			ret_err dlerror();
			std::string GetDLLExtensionName(std::string name);
			dllptr dlopen(const char* filename, int flags);
			int dlclose(dllptr p);
			addrptr dlsym(dllptr p, const char* name);
		}
		void __UnloadLib(const std::string& name, dllptr* lib, refl::store::storage* store);
		void __GenerateLib(const std::string& path, const std::string& name, refl::reflector& r);
		bool __LoadLib(const std::string& name, dllptr* lib, refl::store::storage* store);

		}


	};


