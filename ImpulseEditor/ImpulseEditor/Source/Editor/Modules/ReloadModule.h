#pragma once

#include "EditorModule.h"
#include "Editor/Project/ProjectData.h"
#include "Editor/Project/GenerateProject.h"

namespace Editor {

	class ReloadModule : public EditorModule {

	public:
		ReloadModule(Project::LocalProject* prj);

		~ReloadModule() {};

		bool CanReload();
		bool IsReloading();
		void Reload();

		inline void Create(const std::string& name, bool* is_open, uint32_t flags) override {};
	private:
		void SaveProjects();
		void LoadProjects();
		bool GenerateProject(bool retry = true);
		int project_pos = 0;
		std::mutex m_reloadMutex;
		bool b_isReloading = false;
		Project::LocalProject* m_project;
		std::vector<Project::LocalProject> m_projectData;
		std::string m_lastProjectDir = "";

	};

}
