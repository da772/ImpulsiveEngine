#pragma once
#include <GEngine.h>


struct ProjectData {
	std::string name;
	GEngine::Ref<GEngine::Texture2D> thumbNail;
	std::string path;
	std::string time;
	uint64_t lastModified;
};

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

	std::string m_defaultProjectName = "MyProject";

	char m_search_char[512] = { 0 };
	bool m_createProjectModal = false;
	bool m_confirmDeleteModal = false;

	GEngine::Ref<GEngine::Texture2D> searchIcon;
	GEngine::Ref<GEngine::Texture2D> checkerBoardIcon;
	GEngine::Ref<GEngine::Texture2D> folderIcon;

	void Search();
	void Sort(int i );
	void ImportProject(const std::string& path);
	void CreateNewProjectDialog();
	void CreateDeleteConfirmationDialog();
	void OpenProject(const std::string& path);
	void DeleteProject(const std::string& path);
	ProjectData& GetProjectDataFromPath(const std::string& path);
};

