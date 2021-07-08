#pragma once
#include "EditorModule.h"
#include "Editor/Project/ProjectData.h"


class ImRect;

namespace Editor {

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
		void SelectView(const DirectoryPath& d);
		void ShowSetingsModal(const DirectoryPath& flags);
		void AcceptDirPayload(const DirectoryPath& p, const GEngine::Vector2f& pos);
		void AcceptDirPayloadFolder(const DirectoryPath& p, ImRect* rect);
		void CreateFolderView(const IterativeDirectoryPath& p, float fontSize);
		void GetDirectoryChildren(IterativeDirectoryPath* root);


	private:
		std::filesystem::directory_entry m_directoryBase;
		std::filesystem::directory_entry m_scriptDirectoryBase;
		std::filesystem::directory_entry m_contentDirectoryBase;
		std::filesystem::directory_entry m_currentEntry;
		std::filesystem::directory_entry lastFolderViewEntry;
        bool isDragging = false;
		bool isDragAndDrop = false;
        float lastX = -1;
        float dropDownPanelWidth = .145f;
        std::vector<DirectoryPath> folderViewdirectories = {};
		uint64_t folderViewRefreshTime = 5000;
		uint64_t lastFolderViewUpdate = 0;
		bool forceFolderViewRefresh = true;

		bool dropDownFolderForceRefresh = true;

		std::vector<IterativeDirectoryPath> dropDownDirectories = {};


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
