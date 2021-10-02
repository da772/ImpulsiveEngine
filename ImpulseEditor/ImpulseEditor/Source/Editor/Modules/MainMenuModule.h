#ifdef GE_EDITOR
#pragma once
#include "EditorModule.h"
#include "Editor/Project/GenerateProject.h"
#include "Editor/Project/ProjectData.h"


namespace Editor {

	class ReloadModule;

	class SerializerModule;

	class MainMenuModule : public EditorModule {
	public:

		MainMenuModule(std::unordered_map<std::string, EditorModuleData>* modules, Project::LocalProject* proj, ReloadModule* reloadModule, SerializerModule* serialzier);
		~MainMenuModule();
		void Create(const std::string& name, bool* is_open, uint32_t flags) override;


		bool GenerateProject(bool retry = true);

		bool GenerateModal();

		void LoadProjects();
		void SaveProjects();
	private:
		void SaveAs();
		void Save();
		void New();
		void Open();
		void Build();
		void Undo();
		void Redo();

	private:
		bool m_generateModal = false;
		bool m_generateModalStart = false;
		std::unordered_map<std::string, EditorModuleData>* m_modules;
		Project::LocalProject* m_project;
		size_t project_pos = 0;
		GEngine::Ref<GEngine::Texture2D> folderIcon = nullptr;
		std::string m_msBuildLocation = "";
		char buildBuffer[2048] = { 0 };
		std::string m_lastProjectDir = "";
		std::vector<Project::LocalProject> m_projectData;
		ReloadModule* m_reloadModule;
		SerializerModule* m_serialzierModule;
		uint64_t m_historyValidId;
		bool m_canRedo = false;
		bool m_canUndo = false;

	};



}
#endif