#pragma once

namespace GEngine {

	class GE_API FileData {
	public:
		FileData(uint64_t size, unsigned char* data);
		~FileData();


		inline uint64_t GetDataSize() const { return m_Size ; }
		inline unsigned char* GetData() const {
			return m_Data;
		};

		inline void Clear() {
			if (m_Data != nullptr)
				free(m_Data);
			m_Size = 0;
			m_Data = nullptr;
		}

		inline void SetData(uint64_t size, unsigned char* data) {
			m_Size = size;
			m_Data = data;
		}


	private:
		unsigned char* m_Data = nullptr;
		uint64_t m_Size;
	};


	class GE_API FileSystem {
	public:
		
		static void PakDirectory(std::string src, std::string out, bool srcRelative = true);
		static void Copy(std::string src, std::string out, bool srcRelative = true, bool outRelative = true);
		static void LoadPak(const std::string path, bool setDefault = true);
		static Ref<FileData> LoadFileFromPak(const std::string& path, std::string pak = "");
		static void UnloadPak();
		static void SetDefaultPak(const std::string& pak);
		static std::string FilePath(const std::string& path);
		static std::string GetExecutablePath();
		static std::string GetExecutableDir();
		static std::string GetParentExecuteableDir(int levelsUp);
		static std::string GetDefaultLocation();
		static int ExtractZip(const std::string& zip, const std::string& location);
		static int ZipDir(const std::string& dir, const std::string& out);
		static int ZipFiles(const std::vector<std::string>& , const std::string& out);
		static bool CreateDirectories(const  std::string& dir);
		static bool RemoveAllFolders(const std::string& dir);
		static bool MoveFile(const std::string& file, const std::string& dst);
		static bool DeleteFile(const std::string& file);

		static void OpenFileDialog(const std::vector<std::pair<std::string, std::string>>& filters, std::string& ret, const std::string& startPath = "", bool isFolder = false);

		static Ref<FileData> FileDataFromPath(std::string path, bool fromPak = true, bool relative = false);
		static void AddToMemoryPak(const std::string& name, Ref<FileData> data);
		static bool FileInMemory(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<FileData>> s_fileMap;
		static Ref<FileData> LoadFileFromPakFile(const std::string& file, const std::string pak);
		static std::string s_pakDir;
		static void zip_walk(void* zip, const char* path, size_t dirSize);

	};


	

}


