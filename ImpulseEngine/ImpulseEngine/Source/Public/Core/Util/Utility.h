#pragma once

#include "Public/Core/FileSystem/FileSystem.h"
#include <reflection/reflection.hpp>


#if defined(_WIN32)
using dllptr = HMODULE;
using addrptr = FARPROC;
using ret_err = DWORD;
#elif defined(__APPLE__)
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#elif defined(__linux__) 
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#else
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#endif


namespace GEngine {

	namespace Utility {
	
		GE_API std::string IPV4ToString(uint32_t ip);
		GE_API Vector3f ScreenPosToWorldPos(Vector2f pos);
		GE_API std::string GenerateHash(const int len);
		GE_API void GenerateHash(char* str, const int len);
		namespace sys {

			GE_API std::string exec_command(const std::string& cmd);
			GE_API std::string build_proj(const std::string& dir, const std::string& file);
			GE_API std::string compile_proj(const std::string& dir, const std::string& file);
#ifdef _WIN32
			GE_API PROCESS_INFORMATION CreateChildProcess(const std::string& cmd, HANDLE _ERR_WR, HANDLE _OUT_WR);
			GE_API std::string ReadFromPipe(HANDLE err_RD, HANDLE out_RD);
#endif

		}
		namespace dll {

			GE_API void movePDB(const std::string& dir, const std::string& name, const std::string& newPrefix, bool deleteOld);
			GE_API ret_err dlerror();
			GE_API std::string GetDLLExtensionName(std::string name);
			GE_API dllptr dlopen(const char* filename, int flags);
			GE_API int dlclose(dllptr p);
			GE_API addrptr dlsym(dllptr p, const char* name);
		}
		GE_API void __UnloadLib(const std::string& name, dllptr* lib, refl::store::storage* store);
		GE_API void __GenerateLib(const std::string& path, const std::string& name, refl::reflector& r);
		GE_API bool __LoadLib(const std::string& name, dllptr* lib, refl::store::storage* store);

		}


	};


