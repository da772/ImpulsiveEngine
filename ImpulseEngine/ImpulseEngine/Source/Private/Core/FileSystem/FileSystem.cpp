#include "gepch.h"
#include "Public/Core/FileSystem/FileSystem.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Application/Window.h"
#include <zlib.h>

#define GE_FILESYSTEM_CHAR_LENGTH 1024

#if defined( GE_PLATFORM_WINDOWS)
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#include <Windows.h>
#include <commdlg.h>
#define _HAS_STD_BYTE 0
#include <shlwapi.h>
#include <shlobj.h>
#endif

#ifdef GE_CONSOLE_APP
#include <nfd.h>
#include <zip.h>
#endif

#ifdef GE_PLATFORM_MACOSX
#include "ObjCWrapper/MacUtil.h"
#endif

#ifdef GE_PLATFORM_ANDROID
#include "Public/Platform/Window/Android/android_util.h"
#endif

#ifdef GE_PLATFORM_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif



namespace GEngine {
	
	std::unordered_map<std::string, Ref<FileData>> FileSystem::s_fileMap;
	std::string FileSystem::s_pakDir;

	void FileSystem::PakDirectory(std::string src, std::string out, bool srcRelative)
	{
#if defined(GE_CONSOLE_APP) || defined(GE_SERVER_APP)
		// Initalize varibales
		uint32_t size = 0;
		std::unordered_map<std::string, Ref<FileData>> map;
		// Get Executable dir
		std::string ex = GetExecutableDir();
		if (srcRelative)
			src = FilePath(src);
		else {
			ex = src;
			std::replace(ex.begin(), ex.end(), '\\', '/');
			uint32_t slash = (uint32_t)ex.find_last_of("/");
			if (slash >= 0) {
				ex = ex.substr(0, slash);
			}
		}
		if (srcRelative)
			out = FilePath(out);
		// Iterate through directories
		for (const auto& entry : std::filesystem::recursive_directory_iterator(src)) {
			std::string pth = entry.path().generic_string();
			if (!entry.is_directory() && pth.find(".pak") == pth.npos) {
				std::replace(pth.begin(), pth.end(), '\\', '/');
				std::string relPath = pth;
				if (srcRelative) {
					uint32_t pos = relPath.find(ex);
					relPath.replace(relPath.begin() + (int)pos, relPath.begin() + (int)pos + ex.size() + 1, "");
				}
				Ref<FileData> fd = FileDataFromPath(relPath, false, false);
				if (!srcRelative) {
					uint32_t pos = relPath.find(ex);
					relPath = relPath.substr(pos+ex.size()+1, relPath.size()-pos);
				}
				size += sizeof(FileData)+relPath.size()*sizeof(char);
				
				map[relPath] = fd;
			}
		}

		// Write map to string
		std::stringstream _out(std::ios::out);
		uint32_t _size = map.size();
		_out.write((char*)&_size, sizeof(uint32_t));
		char* _str = new char[GE_FILESYSTEM_CHAR_LENGTH];
		for (auto key : map) {
			memset(_str, 0, GE_FILESYSTEM_CHAR_LENGTH);
			memcpy(_str, &key.first[0], key.first.size());
			uint64_t s = key.second->GetDataSize();
			_out.write((char*)_str, sizeof(char) * GE_FILESYSTEM_CHAR_LENGTH);
			_out.write((char*)&s, sizeof(uint64_t));
			_out.write((char*)key.second->GetData(), key.second->GetDataSize());
		}
		
		// Prepare for compression
		_out.seekg(0, std::ios::end);
		int streamSize = _out.str().size();
		char* buff = (char*)malloc(streamSize);
		std::string buff1 = _out.str();
		_out.clear();

		// Compress string
		z_stream defstream;
		defstream.zalloc = Z_NULL;
		defstream.zfree = Z_NULL;
		defstream.opaque = Z_NULL;
		defstream.avail_in = (uInt)streamSize; 
		defstream.next_in = (Bytef*)buff1.data(); 
		defstream.avail_out = (uInt)streamSize;
		defstream.next_out = (Bytef*)buff;

		deflateInit(&defstream, Z_BEST_COMPRESSION);
		int _res = deflate(&defstream, Z_FINISH);
		deflateEnd(&defstream);

		// Save compressed file
		std::ofstream _outFile(out, std::ios::out | std::ios::binary | std::ios::trunc);
		_outFile.write(buff, defstream.total_out);
		_outFile.close();

		free((void*)buff);
		delete[] _str;
#endif
	}

	void FileSystem::Copy(std::string src, std::string out, bool srcRelative /*= true*/)
	{
#if defined(GE_CONSOLE_APP) || defined(GE_SERVER_APP)
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
		Ref<FileData> fd;
		// Get pak file from disk

		fd = FileDataFromPath(path.c_str(), true, true);
#ifndef GE_PLATFORM_ANDROID
		path = FilePath(path);
#endif

		// Allocate uncompressed data size of compressed data size ( for now )
		unsigned char* dstPtr = (unsigned char*)malloc(fd->GetDataSize());

		// Initalize z_stream
		z_stream infstream;
		infstream.zalloc = Z_NULL;
		infstream.zfree = Z_NULL;
		infstream.opaque = Z_NULL;

		// Uncompress from
		infstream.avail_in = (uInt)fd->GetDataSize();
		infstream.next_in = (Bytef*)fd->GetData();
		// Uncompress to
		infstream.avail_out = (uInt)fd->GetDataSize();
		infstream.next_out = (Bytef*)dstPtr;
		
		// Init stream
		inflateInit(&infstream);
		int _res = Z_OK;
		unsigned long dstSize = fd->GetDataSize();
		// Realloc increment size
		unsigned long incSize = fd->GetDataSize() / 2.f;
		// Wait until we reach end of stream
		while (_res != Z_STREAM_END) {
			// Check for errors
			if (_res != Z_BUF_ERROR && _res != Z_OK) {
				GE_CORE_ASSERT(false, "ZLib Inflation Error: {0}", _res);
				free(dstPtr);
				return;
			}
			// Realloc memory when reaching end of buffer
			if (_res == Z_BUF_ERROR) {
				dstSize += incSize;
				dstPtr = (unsigned char*)realloc(dstPtr, dstSize);
				infstream.next_out = (Bytef*)(dstPtr+infstream.total_out);
				infstream.avail_out = (uInt)dstSize;
			}
			// inflate buffer
			_res = inflate(&infstream, Z_NO_FLUSH);
		}
		inflateEnd(&infstream);

		// Copy buffer into stringstream
		std::stringstream  in;
		in.write((const char*)dstPtr, infstream.total_out);
		// free buffer
		free(dstPtr);

		// Get asset count
		uint32_t count = 0;
		in.read((char*)&count, sizeof(uint32_t));
		uint64_t sz = 0;
		// Loop through assets
		char* name = (char*)malloc(GE_FILESYSTEM_CHAR_LENGTH * sizeof(char));
		for (int i = 0; i < count; i++) {
			uint64_t s = 0;
			char* dat;
			in.read((char*)name, sizeof(char) * 1024);
			in.read((char*)&s, sizeof(uint64_t));
			dat = (char*)malloc(s);
			in.read((char*)dat, s);

			Ref<FileData> filedata = std::make_shared<FileData>(s, (unsigned char*)dat);

			s_fileMap[name] = filedata;
			sz += s;
		}
		free(name);

		if (setDefault)
			SetDefaultPak(path);
		GE_CORE_TRACE("LOADED PAK FILE OF SIZE: {0} MB WITH {1} ENTRIES", sz/1e6f, s_fileMap.size());
	}

	GEngine::Ref<GEngine::FileData> FileSystem::LoadFileFromPakFile(const std::string& file, std::string pak)
	{
		if (pak == "")
			return nullptr;
		pak = FilePath(pak);
		std::ifstream in(pak, std::ios::in | std::ios::binary);
		uint32_t count = 0;
		in.read((char*)&count, sizeof(uint32_t));
		Ref<FileData> fd = nullptr;
		char* name = (char*)malloc(GE_FILESYSTEM_CHAR_LENGTH * sizeof(char));
		char* dat = NULL;
		uint64_t s = 0;
		for (int i = 0; i < count; i++) {
			
			in.read((char*)name, sizeof(char) * 1024);
			in.read((char*)&s, sizeof(uint64_t));
			dat = (char*)realloc(dat, s);
			in.read((char*)dat, s);
			if (std::string(name) == file) {
				fd = std::make_shared<FileData>(s, (unsigned char*)dat);
				break;
			}
			
		}
		free(dat);
		free(name);

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

		if (fromPak) {
			fileData = FileSystem::LoadFileFromPak(path);

			if (fileData != nullptr) {
				GE_CORE_INFO("FILE LOADED FROM PAK: {0}", path);
				return fileData;
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
			GE_CORE_ERROR("FILE COULD NOT BE READ: {0}", path);
			return nullptr;
		}
		else {
			fileData = Ref<FileData>(new FileData(size + 1, (unsigned char*)dat));
		}
#endif
		return fileData;
	}
	
	void FileSystem::AddToMemoryPak(const std::string& name, Ref<FileData> data)
	{
		s_fileMap[name] = data;
	}

	bool FileSystem::FileInMemory(const std::string& name)
	{
		return s_fileMap.find(name) != s_fileMap.end();
	}

	GEngine::Ref<GEngine::FileData> FileSystem::LoadFileFromPak(const std::string& path, std::string pak)
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

	void FileSystem::SetDefaultPak(const std::string& pak)
	{
		s_pakDir = pak;
	}

	std::string FileSystem::FilePath(const std::string& path)
	{
#ifdef GE_PLATFORM_ANDROID
		return path;
#endif
		const std::string& filePath = GetExecutablePath();

		uint32_t slash = (uint32_t)filePath.find_last_of("/\\");

		if (slash >= 0) {
			return filePath.substr(0, slash) + "/" + path;
		}
		GE_CORE_ASSERT(false, "Unable to find /\\ in: {0}", filePath);

		return std::string();
	}

	std::string FileSystem::GetExecutablePath()
	{
#if defined(GE_PLATFORM_WINDOWS )
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
        const std::string& rPath =  std::string(path) +  std::string("/");
		CFRelease(dirURL);
		CFRelease(dirSTR);
        return rPath;
#endif
		return std::string();
	}

	std::string FileSystem::GetExecutableDir()
	{
		const std::string& filePath = GetExecutablePath();

		uint32_t slash = (uint32_t)filePath.find_last_of("/\\");

		if (slash >= 0) {
			return filePath.substr(0, slash);
		}
		GE_CORE_ASSERT(false, "Unable to find /\\ in: {0}", filePath);

		return std::string();
	}
	

	std::string FileSystem::GetParentExecuteableDir(int levelsUp)
	{
		const std::string& exePath = GetExecutableDir();
		std::string dirPath = exePath;

#if defined( GE_PLATFORM_MACOSX)
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

	std::string FileSystem::GetDefaultLocation()
	{
		std::string loc;
#ifdef GE_PLATFORM_WINDOWS
		TCHAR szPath[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
		{
			char sPath[MAX_PATH] = { 0 };
			wcstombs(sPath, szPath, wcslen(szPath) + 1);
			loc = sPath;
		}
#endif
#ifdef GE_PLATFORM_LINUX
		loc = "/etc/";
#endif
#ifdef GE_PLATFORM_MACOSX
		loc = __MASCOSX_GET_APPLICATION_SUPPORT();
#endif
		return loc;
	}

	int FileSystem::ExtractZip(const std::string& zip, const std::string& location)
	{
#ifdef GE_CONSOLE_APP
		Ref<FileData> data = FileDataFromPath(zip);
		if (data != nullptr) {
			int res = zip_stream_extract((const char*)data->GetData(), data->GetDataSize()-1, location.c_str(), NULL, NULL);
			if (res) {
				GE_CORE_ERROR("Failed to extract: {0}", res);
			}
			return res;
		}
		GE_CORE_ERROR("Failed to extract: {0} : File not found", zip);
#endif
		return -1;
	}

	int FileSystem::ZipDir(const std::string& _dir, const std::string& out)
	{
#ifdef GE_CONSOLE_APP
		char* outbuf = NULL;
		ssize_t outbufsize = 0;
		struct zip_t* zip = zip_stream_open(NULL, 0, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
		std::string dir = _dir;
		std::replace(dir.begin(), dir.end(), '\\', '/');
		zip_walk(zip, dir.c_str(), dir.size());
		zip_stream_copy(zip, (void**)&outbuf, &outbufsize);
		zip_stream_close(zip);
		std::ofstream outS(out, std::ios::binary | std::ios::out | std::ios::trunc);
		outS.write(outbuf, outbufsize);
		outS.close();
		free(outbuf);
#endif
		return 0;
	}

	void FileSystem::zip_walk(void* _zip, const char* path, size_t dirSize)
	{
#ifdef GE_CONSOLE_APP
		zip_t* zip = (zip_t*)_zip;
		std::string dir = path;
		dir.erase(0, dirSize);

		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
			// skip "." and ".."
			std::string pth = entry.path().generic_string();
			
			if (entry.is_directory()) {
				
			}
			else {
				zip_entry_open(zip, &pth[dirSize+1]);
				zip_entry_fwrite(zip, pth.c_str());
				zip_entry_close(zip);
			}
		}
#endif
	}

	void FileSystem::CreateDirectories(const std::string& dir)
	{
#ifdef GE_CONSOLE_APP
		std::filesystem::create_directories(dir);
#endif
	}

	void FileSystem::RemoveAllFolders(const std::string& dir)
	{
#ifdef GE_CONSOLE_APP
		std::filesystem::remove_all(dir);
#endif
	}

	void FileSystem::OpenFileDialog(const std::vector<std::pair<std::string, std::string>>& filters, std::string& ret, const std::string& _startPath, bool isFolder)
	{
#ifdef GE_CONSOLE_APP
		NFD_Init();

		nfdchar_t* outPath;
		nfdresult_t result;
		nfdchar_t* startPath = NULL;
		if (_startPath.size() > 0) {
			startPath = (char*)_startPath.data();
		}

		if (!isFolder) {
			nfdfilteritem_t* filterItem = (nfdfilteritem_t*)malloc(sizeof(nfdfilteritem_t) * filters.size());// { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };

			for (int i = 0; i < filters.size(); i++) {
				const std::pair<std::string, std::string>& p = filters[i];
				filterItem[i] = { p.first.data(), p.second.data() };
			}

			

			result = NFD_OpenDialog(&outPath, filterItem, filters.size(), startPath);
		}
		else {
			result = NFD_PickFolder(&outPath, startPath);
		}
		if (result == NFD_OKAY)
		{
			ret = outPath;
			NFD_FreePath(outPath);
		}
		else if (result == NFD_CANCEL)
		{

		}
		else
		{
			GE_CORE_DEBUG("Error: {0}", NFD_GetError());
		}

		NFD_Quit();
#endif

	}

}
