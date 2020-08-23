#include "gepch.h"
#include "Public/Core/FileSystem/FileSystem.h"

#define GE_FILESYSTEM_CHAR_LENGTH 1024

#ifdef GE_PLATFORM_WINDOWS
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#endif

#ifdef GE_PLATFORM_ANDROID
#include "Public/Core/Platform/Window/Android/android_util.h"
#endif

#ifdef GE_PLATFORM_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif


namespace GEngine {
	
	std::unordered_map<std::string, Ref<FileData>> FileSystem::s_fileMap;
	std::string FileSystem::s_pakDir;

	void FileSystem::PakDirectory(std::string src, std::string out, bool srcRelative)
	{
#ifdef GE_CONSOLE_APP
		uint32_t size = 0;
		std::unordered_map<std::string, Ref<FileData>> map;
		std::string ex = GetExecutableDir();
		if (srcRelative)
			src = FilePath(src);
		else {
			ex = src;
			uint32_t slash = (uint32_t)ex.find_last_of("/\\");
			if (slash >= 0) {
				ex = ex.substr(0, slash);
			}
		}
		if (srcRelative)
			out = FilePath(out);
		for (const auto& entry : std::filesystem::recursive_directory_iterator(src)) {
			std::string pth = entry.path().generic_string();
			if (!entry.is_directory() && pth.find(".pak") == pth.npos) {
				std::replace(pth.begin(), pth.end(), '\\', '/');
				std::string relPath = pth;
				if (srcRelative) {
					uint32_t pos = relPath.find(ex);
					relPath.replace(relPath.begin() + (int)pos, relPath.begin() + (int)pos + ex.size() + 1, "");
				}
				Ref<FileData> fd = FileDataFromPath(relPath, true, false);
				if (!srcRelative) {
					uint32_t pos = relPath.find(ex);
					relPath.replace(relPath.begin() + (int)pos, relPath.begin() + (int)pos + ex.size() + 1, "");
				}
				size += sizeof(FileData)+relPath.size()*sizeof(char);
				
				map[relPath] = fd;
			}
		}

		std::ofstream _out(out, std::ios::out | std::ios::binary | std::ios::trunc);
		uint32_t _size = map.size();
		_out.write((char*)&_size, sizeof(uint32_t));
		for (auto key : map) {
			uint64_t s = key.second->GetDataSize();
			_out.write((char*)key.first.c_str(), sizeof(char) * GE_FILESYSTEM_CHAR_LENGTH);
			_out.write((char*)&s, sizeof(uint64_t));
			_out.write((char*)key.second->GetData(), key.second->GetDataSize());
		}
		_out.close();
#endif
	}

	void FileSystem::Copy(std::string src, std::string out, bool srcRelative /*= true*/)
	{
#ifdef GE_CONSOLE_APP
		std::string ex = GetExecutableDir();
		if (srcRelative)
			src = FilePath(src);
		else {
			ex = src;
			uint32_t slash = (uint32_t)ex.find_last_of("/\\");
			if (slash >= 0) {
				ex = ex.substr(0, slash);
			}
		}
		if (srcRelative)
			out = FilePath(out);

		std::filesystem::copy(src, out, std::filesystem::copy_options::overwrite_existing);
#endif
	}

	void FileSystem::LoadPak(std::string path, bool setDefault)
	{
		UnloadPak();

#ifdef GE_PLATFORM_ANDROID
		Ref<FileData> fd = FileDataFromPath(path.c_str());
		std::stringstream  in;
		in.write((const char*)fd->GetDataAsString(), fd->GetStringSize());
#else
		path = FilePath(path);
		std::ifstream in(path, std::ios::in | std::ios::binary);
#endif
		
		uint32_t count = 0;
		in.read((char*)&count, sizeof(uint32_t));
		uint64_t sz = 0;

		for (int i = 0; i < count; i++) {
			char* name = (char*)malloc(GE_FILESYSTEM_CHAR_LENGTH * sizeof(char));
			uint64_t s = 0;
			char* dat;
			in.read((char*)name, sizeof(char) * 1024);
			in.read((char*)&s, sizeof(uint64_t));
			dat = (char*)malloc(s);
			in.read((char*)dat, s);

			Ref<FileData> fd = make_shared<FileData>(s, (unsigned char*)dat);

			s_fileMap[name] = fd;
			sz += s;
			free(name);
		}
#ifndef GE_PLATFORM_ANDROID
		in.close();
#endif
		if (setDefault)
			SetDefaultPak(path);
		GE_CORE_TRACE("LOADED PAK FILE OF SIZE: {0} MB WITH {1} ENTRIES", sz/1e6f, s_fileMap.size());
	}

	GEngine::Ref<GEngine::FileData> FileSystem::LoadFileFromPakFile(std::string file, std::string pak)
	{
		if (pak == "")
			return nullptr;
		pak = FilePath(pak);
		std::ifstream in(pak, std::ios::in | std::ios::binary);
		uint32_t count = 0;
		in.read((char*)&count, sizeof(uint32_t));
		Ref<FileData> fd = nullptr;

		for (int i = 0; i < count; i++) {
			char* name = (char*) malloc(GE_FILESYSTEM_CHAR_LENGTH * sizeof(char));
			uint64_t s = 0;
			char* dat;
			in.read((char*)name, sizeof(char) * 1024);
			in.read((char*)&s, sizeof(uint64_t));
			dat = (char*)malloc(s);
			in.read((char*)dat, s);
			if (name == file) {
				fd = make_shared<FileData>(s, (unsigned char*)dat);
				free(name);
				break;
			}

			free(dat);
			free(name);
		}

		in.close();
		return fd;
	}

	FileData::FileData(uint64_t size, unsigned char* data)
	{
		m_Size = size;
		m_Data = data;
	}

	FileData::~FileData()
	{
		if (m_Data != nullptr) {
			free(m_Data);
			m_Data = nullptr;
		}
	}

	Ref<FileData> FileSystem::FileDataFromPath(std::string path, bool fromPak, bool relative)
	{
		Ref<FileData> fileData;

		if (!fromPak) {
			fileData = FileSystem::LoadFileFromPak(path);

			if (fileData != nullptr) {
				GE_CORE_TRACE("FILE LOADED FROM PAK: {0}", path);
				return fileData;
			}
			else {
				GE_CORE_ERROR("FILE NOT LOADED FROM PAK: {0}", path);
			}
		}

		if (relative)
			path = FilePath(path);

#ifdef GE_PLATFORM_ANDROID
		AndroidUtil::filedat f = AndroidUtil::LoadFileToMemory(path.c_str());
		fileData = Ref<FileData>(new FileData(f.size + 1, (unsigned char*)f.data));
#else
		std::ifstream file(path, std::ios::binary | std::ios::ate | std::ios::in);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		char* dat = (char*)malloc(size + 1);

		if (!file.read(dat, size))
		{
			free(dat);
			GE_CORE_ASSERT(false, "FILE COULD NOT BE READ");
		}
		else {
			fileData = Ref<FileData>(new FileData(size + 1, (unsigned char*)dat));
		}
#endif
		return fileData;
	}
	
	GEngine::Ref<GEngine::FileData> FileSystem::LoadFileFromPak(std::string path, std::string pak)
	{
		std::unordered_map<std::string, Ref<FileData>>::iterator it = s_fileMap.find(path);

		if (it == s_fileMap.end()) {
			if (pak == "")
				pak = s_pakDir;
			GE_CORE_WARN("{0} loaded from disk.", path);
			return LoadFileFromPakFile(path, pak);
		}
		else {
			return it->second;
		}
	}

	void FileSystem::UnloadPak()
	{
		s_fileMap.clear();
	}

	void FileSystem::SetDefaultPak(std::string pak)
	{
		s_pakDir = pak;
	}

	std::string FileSystem::FilePath(std::string path)
	{
#ifdef GE_PLATFORM_ANDROID
		return path;
#endif
		std::string filePath = GetExecutablePath();

		uint32_t slash = (uint32_t)filePath.find_last_of("/\\");

		if (slash >= 0) {
			return filePath.substr(0, slash) + "/" + path;
		}
		GE_CORE_ASSERT(false, "Unable to find /\\ in: {0}", filePath);

		return std::string();
	}

	std::string FileSystem::GetExecutablePath()
	{
#ifdef GE_PLATFORM_WINDOWS 
#pragma warning(push)
#pragma warning(disable : 4996)
		TCHAR NPath[MAX_PATH];
		GetModuleFileName(NULL, NPath, MAX_PATH);
		std::wstring w_arr(NPath);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string path = converter.to_bytes(w_arr);
		std::replace(path.begin(), path.end(), '\\', '/');
		return path;
#pragma warning(pop)
#endif
#if defined(GE_PLATFORM_MACOSX)
		int ret;
		pid_t pid;
		char pathBuf[PROC_PIDPATHINFO_MAXSIZE];
		pid = getpid();
		ret = proc_pidpath(pid, pathBuf, sizeof(pathBuf));

		return std::string(pathBuf);
#endif
#if defined(GE_PLATFORM_LINUX)

		char buff[PATH_MAX];
		ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
		if (len != -1) {
			buff[len] = '\0';
			return std::string(buff);
		}

#endif

#if defined(GE_PLATFORM_IOS)
		//Get a reference to the main bundle
		CFBundleRef mainBundle = CFBundleGetMainBundle();

		// Get a reference to the file's URL
		CFURLRef dirURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
		//CFBundleGetValueForInfoDictionaryKey(mainBundle, CFSTR("CFBundleDisplayName") );

		CFStringRef dirSTR = CFURLCopyFileSystemPath(dirURL, kCFURLPOSIXPathStyle);
		// Get the system encoding method
		CFStringEncoding encodingMethod = CFStringGetSystemEncoding();

		// Convert the string reference into a C string
		const char* path = CFStringGetCStringPtr(dirSTR, encodingMethod);
		// GE_CORE_DEBUG("{0}", path);
        std::string rPath =std::string(path) + std::string("/");
        free((void*)dirSTR);
        return rPath;
#endif
		return std::string();
	}

	std::string FileSystem::GetExecutableDir()
	{
		std::string filePath = GetExecutablePath();

		uint32_t slash = (uint32_t)filePath.find_last_of("/\\");

		if (slash >= 0) {
			return filePath.substr(0, slash);
		}
		GE_CORE_ASSERT(false, "Unable to find /\\ in: {0}", filePath);

		return std::string();
	}
	

	std::string FileSystem::GetParentExecuteableDir(int levelsUp)
	{
		std::string exePath = GetExecutableDir();
		std::string dirPath = exePath;
#ifdef GE_PLATFORM_MACOSX
        levelsUp+=3;
#endif
		for (int i = 0; i < levelsUp; i++) {
			uint32_t slash = (uint32_t)dirPath.find_last_of("/\\");
			if (slash >= 0) {
				dirPath = dirPath.substr(0, slash);
			} 
			else {
				break;
			}
		}

		return dirPath+"/";
	}

}
