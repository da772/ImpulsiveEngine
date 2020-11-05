#pragma once

namespace GEngine {

	class FileData {
	public:
		FileData(uint64_t size, unsigned char* data);
		~FileData();


		inline uint64_t GetDataSize() const { return m_Size - 1; }
		inline uint64_t GetStringSize() const { return m_Size; }
		inline unsigned char* GetData() const {
			m_Data[m_Size - 1] = 0;
			return m_Data;
		};
		inline unsigned char* GetDataAsString() const {
			m_Data[m_Size - 1] = '\0';
			return m_Data;
		};

		inline void Clear() {
			if (m_Data != nullptr)
				free(m_Data);
			m_Size = 0;
			m_Data = nullptr;
		}


	private:
		unsigned char* m_Data = nullptr;
		uint64_t m_Size;
	};


	class FileSystem {
	public:
		
		static void PakDirectory(std::string src, std::string out, bool srcRelative = true);
		static void Copy(std::string src, std::string out, bool srcRelative = true);
		static void LoadPak(const std::string path, bool setDefault = true);
		static Ref<FileData> LoadFileFromPak(const std::string& path, std::string pak = "");
		static void UnloadPak();
		static void SetDefaultPak(const std::string& pak);
		static std::string FilePath(const std::string& path);
		static std::string GetExecutablePath();
		static std::string GetExecutableDir();
		static std::string GetParentExecuteableDir(int levelsUp);

		static Ref<FileData> FileDataFromPath(std::string path, bool fromPak = false, bool relative = true);

	private:
		static std::unordered_map<std::string, Ref<FileData>> s_fileMap;
		static Ref<FileData> LoadFileFromPakFile(const std::string& file, const std::string pak);
		static std::string s_pakDir;

	};


	

}


