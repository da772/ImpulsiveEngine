
#include "gepch.h"

#include "Public/Core/Util/Utility.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Controller/CameraController.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if defined(GE_MINGW_) || defined (GE_PLATFORM_LINUX)
#include <arpa/inet.h>
#endif

#ifdef GE_PLATFORM_ANDROID
#include "Public/Platform/Window/Android/android_util.h"
#include <arpa/inet.h>
#endif

#ifdef GE_PLATFORM_IOS
#include <CoreFoundation/CoreFoundation.h>
#include <arpa/inet.h>
#endif

#ifdef GE_PLATFORM_MACOSX
#include <arpa/inet.h>
#endif

#if defined(_WIN32)
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#include <algorithm>
#include <libloaderapi.h>
#include <windows.h>
#include <errhandlingapi.h>
#include <process.h>
#define MS_xstr(a) MS_str(a)
#define MS_str(a) #a
#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
#include <libproc.h>
#include <dlfcn.h>
#endif

#if defined(__linux__) 
#include <unistd.h>
#include <dlfcn.h>
#endif

#include <chrono>

static std::string __DLL_prefix = "cpy_";
static bool _renameDLL = false;



namespace GEngine {

	std::string Utility::IPV4ToString(uint32_t ip)
	{
#if defined( GE_PLATFORM_WINDOWS)
		char ch[255];
		inet_ntop(AF_INET, &ip, ch, 255);
		return std::string(ch);
#else 
		char ch[255];
		inet_ntop(AF_INET, &ip, ch, 255);
		return std::string(ch);
#endif
	}


	Vector3f Utility::ScreenPosToWorldPos(Vector2f pos)
	{
		Vector3f position = { pos.x,  pos.y, 1 };
		int width, height;
		GEngine::Application::GetApp()->GetWindow()->GetFrameBufferSize(&width, &height);
		//width = GEngine::Application::GetApp()->GetWindow()->GetWidth();
		//height = GEngine::Application::GetApp()->GetWindow()->GetHeight();
		position = Vector3f(glm::value_ptr(glm::unProject(glm::vec3(position.x, position.y, position.z), Application::GetApp()->GetTargetCamera()->GetViewMatrix(),
			Application::GetApp()->GetTargetCamera()->GetProjectionMatrix(), glm::vec4(0, height, width, -height))));
		return position;
	}

	const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string Utility::GenerateHash(const int len)
	{
		std::string s;
		for (int i = 0; i < len; ++i) {
			s += alphanum[std::rand() % (sizeof(alphanum) - 1)];
		}
		return s;
	}

	void Utility::GenerateHash(char* str, const int len)
	{
		for (int i = 0; i < len; ++i) {
			str[i] = alphanum[std::rand() % (sizeof(alphanum) - 1)];
		}
	}

	void Utility::dll::movePDB(const std::string& dir, const std::string& name, const std::string& newPrefix, bool deleteOld)
	{
#ifdef GE_HOT_RELOAD
		std::string loc = dir + newPrefix + name + ".pdb";
		std::ifstream  src(dir + name + ".pdb", std::ios::binary);
		std::ofstream  dst(loc, std::ios::binary);
		if (src.fail()) {
			GE_CORE_ERROR("Native Generate: Could not move pdb file (Locked)");
			return;
		}
		dst << src.rdbuf();
		dst.close();
		src.close();
		if (deleteOld)
			::remove((dir + name + ".pdb").c_str());
#endif
	}

	std::string Utility::dll::GetDLLExtensionName(std::string name)
	{
#ifdef GE_HOT_RELOAD

#if defined(__linux__)
		return "lib" + name + ".so";
#endif
#if defined(__APPLE__)
		return "lib" + name + ".dylib";
#endif
#ifdef _WIN32
		return name + ".dll";
#endif
#else
		return "";
#endif

	}

	dllptr Utility::dll::dlopen(const char* filename, int flags)
	{
#ifdef GE_HOT_RELOAD
#if defined(__linux__) || defined(__APPLE__)
		return ::dlopen(filename, RTLD_NOW);
#endif
#ifdef _WIN32
		return ::LoadLibraryA(filename);
#endif
#else
		return 0;
#endif
	}

    ret_err Utility::dll::dlerror() {
#ifdef GE_HOT_RELOAD
#if defined(__linux__) || defined(__APPLE__)
        return ::dlerror();
#endif
#if defined(_WIN32)
        return GetLastError();
#endif
#else
        return "";
#endif
    }

	int Utility::dll::dlclose(dllptr p)
	{
#ifdef GE_HOT_RELOAD
#if defined(__linux__) || defined(__APPLE__)
		return ::dlclose(p);
#endif
#ifdef _WIN32
		return (int)FreeLibrary(p);
#endif
#else
		return 0;
#endif
	}

	addrptr Utility::dll::dlsym(dllptr p, const char* name)
	{
#ifdef GE_HOT_RELOAD
#if defined(__linux__) || defined(__APPLE__)
		return ::dlsym(p, name);
#endif
#ifdef _WIN32 
		return ::GetProcAddress(p, name);
#endif
#else
		return 0;
#endif
	}

	std::string GEngine::Utility::sys::build_proj(const std::string& dir, const std::string& file)
	{
#ifdef GE_HOT_RELOAD
		std::string cmd = "";
#if defined(__linux__) || defined(__APPLE__)
		cmd += "cd \"" + dir + "\" && \"./" + file;
#ifdef __APPLE__
		cmd += "_Mac.command\" ";
#endif
#ifdef __linux__
		cmd += "_Linux.sh\" ";
#endif
#endif
#if defined(_WIN32)
		std::string drive = dir.substr(0, 2);
		cmd += dir + file + "_Windows.bat";
		//cmd += "" + dir + "build\" && \""+file+"_Windows.bat\" ";
#endif
		GE_CORE_INFO("Native Build Command: {0}", cmd);
		return Utility::sys::exec_command(cmd);
	}

	std::string GEngine::Utility::sys::compile_proj(const std::string& dir, const std::string& file) {
		::std::string cmd = "";
#ifdef _WIN32
		std::string _msBin = MS_xstr(MS_BUILD_BIN);
		_msBin.erase(_msBin.size() - 1);
		cmd += _msBin + "\\MSBuild.exe\" \"" + dir + file + ".vcxproj\" /verbosity:quiet /nologo ";
#endif
#if defined (__linux__) || defined (__APPLE__)
		cmd += "cd " + dir + " && make -s ";
#endif
#ifdef GE_RELEASE
#if defined (__linux__) || defined (__APPLE__)
		cmd += "config=release";
#endif
#ifdef _WIN32
		cmd += "/p:Configuration=Release ";
#endif
#elif defined(GE_DEBUG)

#if defined (__linux__) || defined (__APPLE__)
		cmd += "config=debug";
#endif
#ifdef _WIN32
		cmd += "/p:Configuration=Dist ";
#endif
#elif defined (GE_DIST) 

#if defined (__linux__) || defined (__APPLE__)
		cmd += "config=debug";
#endif
#ifdef _WIN32
		cmd += "/p:Configuration=Dist ";
#endif
#endif

#if defined (__linux__) || defined (__APPLE__)
		//cmd += "_x86_64";
#endif
#ifdef _WIN32
		cmd += "/p:Platform=x64 ";
#endif
		/*
			else if (arch == "x86") {
	#if defined (__linux__) || defined (__APPLE__)
				cmd += "_x86";
	#endif
	#ifdef _WIN32
				cmd += "/p:Platform=Win32 ";
	#endif
			}
		*/
		GE_CORE_INFO("Native Compile Command: {0}", cmd);
		return Utility::sys::exec_command(cmd);
#else
		return "";
#endif

	}

	std::string GEngine::Utility::sys::exec_command(const std::string& cmd)
	{
#ifdef GE_HOT_RELOAD
#if defined(__linux__) || defined (__APPLE__)
		char buffer[2048] = { 0 };
		std::string result = "";

		// Open pipe to file
		FILE* pipe = popen((cmd + " 2>&1").c_str(), "r");
		if (!pipe) {
			return "popen failed!";
		}

		// read till end of process:
		while (!feof(pipe)) {
			// use buffer to read and add to result
			if (fgets(buffer, 128, pipe) != NULL) {
				result += buffer;
			}
		}
		pclose(pipe);
		return result;
#endif
#ifdef _WIN32
		std::string output = "";
		HANDLE out_RD, out_WR, err_RD, err_WR;
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		// Create a pipe for the child process's STDERR. 
		if (!CreatePipe(&err_RD, &err_WR, &sa, 0)) {
			return "FAILED TO CREATE ERROR PIPE";
		}
		// Ensure the read handle to the pipe for STDERR is not inherited.
		if (!SetHandleInformation(err_RD, HANDLE_FLAG_INHERIT, 0)) {
			return "FAILED TO HANDLE ERROR PIPE INFORMATION";
		}
		// Create a pipe for the child process's STDOUT. 
		if (!CreatePipe(&out_RD, &out_WR, &sa, 0)) {
			return "FAILED TO CREATE OUT PIPE";
		}
		// Ensure the read handle to the pipe for STDOUT is not inherited
		if (!SetHandleInformation(out_RD, HANDLE_FLAG_INHERIT, 0)) {
			return "FAILED TO HANDLE OUT PIPE INFORMATION";
		}

		PROCESS_INFORMATION info = Utility::sys::CreateChildProcess(cmd, err_WR, out_WR);
		output = Utility::sys::ReadFromPipe(err_RD, out_RD);
		return output;
#endif
#else
		return "";
#endif
	}

#ifdef _WIN32
	PROCESS_INFORMATION GEngine::Utility::sys::CreateChildProcess(const std::string& cmd, HANDLE _ERR_WR, HANDLE _OUT_WR) {
		PROCESS_INFORMATION piProcInfo;
		STARTUPINFOA siStartInfo;
		bool bSuccess = FALSE;

		// Set up members of the PROCESS_INFORMATION structure. 
		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

		// Set up members of the STARTUPINFO structure. 
		// This structure specifies the STDERR and STDOUT handles for redirection.
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdError = _ERR_WR;
		siStartInfo.hStdOutput = _OUT_WR;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		LPSTR cmdLine = const_cast<char*>(cmd.c_str());

		// Create the child process. 
		bSuccess = CreateProcessA(NULL,
			cmdLine,     // command line 
			NULL,          // process security attributes 
			NULL,          // primary thread security attributes 
			TRUE,          // handles are inherited 
			0,             // creation flags 
			NULL,          // use parent's environment 
			NULL,          // use parent's current directory 
			&siStartInfo,  // STARTUPINFO pointer 
			&piProcInfo);  // receives PROCESS_INFORMATION

		DWORD d = WaitForSingleObject(piProcInfo.hProcess, INFINITE);

		CloseHandle(_ERR_WR);
		CloseHandle(_OUT_WR);

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		// If an error occurs, exit the application. 
		if (!bSuccess) {
			GE_CORE_ERROR("Creating ChildProcess Failed: {0}", GetLastError());
		}
		return piProcInfo;
	}
#endif

#ifdef _WIN32
	std::string GEngine::Utility::sys::ReadFromPipe(HANDLE err_RD, HANDLE out_RD) {
		DWORD dwRead;
		CHAR* chBuf = (CHAR*)calloc(2048, sizeof(CHAR));
		bool bSuccess = FALSE;
		std::string out = "", err = "";
		for (;;) {
			bSuccess = ReadFile(out_RD, chBuf, 2048, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;

			std::string s(chBuf, dwRead);
			out += s;
		}
		dwRead = 0;
		for (;;) {
			bSuccess = ReadFile(err_RD, chBuf, 2048, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;

			std::string s(chBuf, dwRead);
			err += s;

		}

		CloseHandle(err_RD);
		CloseHandle(out_RD);
		free(chBuf);
		if (err.size() > 0) {
			return err;
		}
		else {
			return out;
		}

	}
#endif
	bool Utility::__LoadLib(const std::string& name, dllptr* lib, refl::store::storage* store)
	{
#ifdef GE_HOT_RELOAD
		if (*lib) {
			GE_CORE_ERROR("Native Library already loaded (Unload before loading)");
			return false;
		}

		std::string loc = FileSystem::GetParentExecuteableDir(0) + Utility::dll::GetDLLExtensionName((_renameDLL ? __DLL_prefix : "") + name);
		if (_renameDLL) {
			std::ifstream src(FileSystem::GetParentExecuteableDir(0) + Utility::dll::GetDLLExtensionName(name), std::ios::binary);

			std::ofstream  dst(loc, std::ios::binary);
			dst << src.rdbuf();
			dst.close();
		}
#ifdef _WIN32
		Utility::dll::movePDB(FileSystem::GetParentExecuteableDir(0), name, "_", true);
#endif
		* lib = Utility::dll::dlopen(loc.c_str(), 0);

		if (!*lib) {
			GE_CORE_ERROR("Native Library: Could not load library - {0}:{1}", name, Utility::dll::dlerror());
			return false;
		}

		void (*func_ptr)(::refl::store::storage*) = reinterpret_cast<void (*)(::refl::store::storage*)>(Utility::dll::dlsym(*lib, "__ReflectionMap__loadGeneratedFiles"));
		if (!func_ptr) {
			GE_CORE_ERROR("Native Library: Could not load symbol - {0}", "__ReflectionMap__loadGeneratedFiles");
			Utility::dll::dlclose(*lib);
			lib = 0;
			return false;
		}
		(*func_ptr)(store);
		return true;
#else
		return false;
#endif

	}

	void Utility::__UnloadLib(const std::string& name, dllptr* lib, refl::store::storage* store)
	{
#ifdef GE_HOT_RELOAD
		if (*lib) {
			void (*func_ptr)(::refl::store::storage*) = reinterpret_cast<void (*)(::refl::store::storage*)>(Utility::dll::dlsym(*lib, "__ReflectionMap__unloadGeneratedFiles"));
			if (func_ptr) {
				(*func_ptr)(store);
			}
			else {
				GE_CORE_ERROR("Native Library: Could not load symbol - {0}", "__ReflectionMap__unloadGeneratedFiles");
			}
			int i = dll::dlclose(*lib);
            GE_CORE_ERROR("DLCOSE ERR: {0}", i);
			*lib = 0;
			std::string loc = FileSystem::GetParentExecuteableDir(0) + Utility::dll::GetDLLExtensionName((_renameDLL ? __DLL_prefix : "") + name);
			if (_renameDLL)
				::remove(loc.c_str());	
		}
#endif
	}

	void Utility::__GenerateLib(const std::string& path, const std::string& name, refl::reflector& r)
	{
#ifdef GE_HOT_RELOAD
		std::string in = path + name;
		std::ifstream t(in);
		std::stringstream buffer;
		buffer << t.rdbuf();
		GE_CORE_INFO("Native Generate: {0}", in);
		r.Generate(in.c_str());
#endif
	}
}
