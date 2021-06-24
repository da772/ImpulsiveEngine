#pragma once
#include "EditorModule.h"

namespace Editor {

	struct DirectoryPath {
		std::string path;
		std::string name;
		std::string ext;
		bool is_directory;
		bool is_empty;
	};

	struct DirectoryRecurse {
		DirectoryPath self;
		std::vector<DirectoryRecurse> children;
	};


	class DirectoryModule : public EditorModule {

	public:
		DirectoryModule(const std::string& directoryBase);
		std::string Create(bool* is_open, uint32_t flags);

	protected:
		std::string moduleName = "Content Browser";


	private:
		void FolderViewPanel();
		void DropDownViewPanel();
		void GetChildren(const std::filesystem::directory_entry& entry);
		void SelectView(const DirectoryPath& d, int fl);

	private:
		std::filesystem::directory_entry m_directoryBase;
		std::filesystem::directory_entry m_currentEntry;

		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		
		std::string m_selectedEntry;
		std::string m_selectedViewEntry;

		int depth = 0;
		int lastDepth = 0;

	};






}
