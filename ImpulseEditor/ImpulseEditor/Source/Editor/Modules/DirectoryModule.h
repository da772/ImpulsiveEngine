#pragma once
#include "EditorModule.h"
#include "Editor/Project/ProjectData.h"

namespace Editor {

	struct DirectoryPath {
		std::string path;
		std::string name;
		std::string ext;
		bool is_directory;
		bool is_empty;

		inline bool operator ==(const DirectoryPath& other) {
			return other.path == path;
		}

		inline bool operator ==(const std::string& other) {
			return other == path;
		}
		inline bool operator !=(const DirectoryPath& other) {
			return other.path != path;
		}

	};

	struct DirectoryPayload {

		inline DirectoryPayload(const DirectoryPath& p) {
			memcpy(path, p.path.c_str(), p.path.size());
			memcpy(name, p.name.c_str(), p.name.size());
			is_directory = p.is_directory;
			is_empty = is_empty;
		}

		char path[2048] = { '\0' };
		char name[256] = { '\0' };
		bool is_directory;
		bool is_empty;

	};

	struct DirectoryRecurse {
		DirectoryPath self;
		std::vector<DirectoryRecurse> children;
	};


	class DirectoryModule : public EditorModule {

	public:
		DirectoryModule(const std::string& directoryBase, Project::ProjectData* projectData);
		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	private:
		enum class PopupFlags : uint32_t {
			FILE = 0x01,
			FOLDER = 0x02,
			UNDEFINED
		};

	private:
		void FolderViewPanel();
		void DropDownViewPanel();
        void ResizePanel();
		void SettingsModal();
		void Filterbar();
		void GetChildren(const std::filesystem::directory_entry& entry);
		void SelectView(const DirectoryPath& d);
		void ShowSetingsModal(const DirectoryPath& flags);
		void AcceptDirPayload(const DirectoryPath& p);
		void CreateFolderView(const DirectoryPath& p, int fl, float fontSize);


	private:
		std::filesystem::directory_entry m_directoryBase;
		std::filesystem::directory_entry m_scriptDirectoryBase;
		std::filesystem::directory_entry m_contentDirectoryBase;
		std::filesystem::directory_entry m_currentEntry;
        bool isDragging = false;
		bool isDragAndDrop = false;
        float lastX = -1;
        float dropDownPanelWidth = .145f;
        std::vector<DirectoryPath> directories = {};
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		char filterBuffer[255] = { 0 };
		char renameBuffer[255] = { 0 };
		bool rename = false;
		std::string m_selectedEntry;
		std::string m_selectedViewEntry;

		Project::ProjectData* m_projectData = nullptr;


		DirectoryPath m_rightClicked;

		float dirWindowWidth = 1;
		
		int depth = 0;
		int lastDepth = 0;

	};






}
