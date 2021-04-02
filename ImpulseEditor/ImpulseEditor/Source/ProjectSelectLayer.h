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
	std::string m_sortType = m_sortTypes[0];
	char m_search_char[512] = { 0 };

	GEngine::Ref<GEngine::Texture2D> searchIcon;

	void Search();
	void Sort(int i );
};

