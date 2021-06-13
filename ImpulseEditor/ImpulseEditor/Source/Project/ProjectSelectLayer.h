#pragma once
#include <GEngine.h>
#include "ProjectData.h"

namespace Project {
	class ProjectSelectLayer : public GEngine::Layer
	{
	public:
		ProjectSelectLayer(const std::string& name);
		~ProjectSelectLayer();
		virtual void Begin() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(GEngine::Timestep timeStep) override;
		virtual void OnEvent(GEngine::Event& event) override;
		virtual void OnImGuiRender() override;
		virtual void OnDraw() override;
		virtual void End() override;


	private:
		std::vector<ProjectData> m_projectData;
		std::vector<ProjectData> m_projectData_Searched;
		std::string selectedProject = "";
		std::vector<std::string> m_sortTypes = { "Last Modified" , "Name", "Directory" };
		int m_sortType = 0;
		std::string m_newProjectName;
		std::string m_newProjectLocation;
		std::string m_newProjectError;
		uint32_t m_newProjectLanguage = 0x01;
		uint32_t m_generatePlatform = 0x01;
		uint64_t m_generateFlags = 0x00;
		uint32_t m_generateBuild = 0x00;

		std::string m_defaultProjectName = "MyProject";
		std::string m_lastProjectDir;

		char m_search_char[512] = { 0 };
		bool m_createProjectModal = false;
		bool m_confirmDeleteModal = false;
		bool m_generateModal = false;
		bool m_deleteFail = false;

		GEngine::Ref<GEngine::Texture2D> searchIcon;
		GEngine::Ref<GEngine::Texture2D> checkerBoardIcon;
		GEngine::Ref<GEngine::Texture2D> folderIcon;

		void Search();
		void Sort(int i);
		void SaveProjects();
		void LoadProjects();
		void ImportProject(const std::string& path);
		void CreateNewProjectDialog();
		void CreateDeleteConfirmationDialog();
		void CreateGenerateDialog();
		void CreateProject(ProjectData* d);
		void GenerateProject(bool retry = true);
		void ShowProject(const std::string& path);
		void OpenProject(const std::string& path);
		bool DeleteProject(const std::string& path);
		void RemoveProject(const std::string& path);
		ProjectData* GetProjectDataFromPath(const std::string& path);

	};

}
