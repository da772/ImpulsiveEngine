#pragma once
#include <GEngine.h>


struct ProjectData {
	std::string name;
	GEngine::Ref<GEngine::Texture2D> thumbNail;
	std::string path;
	std::string time;
	uint64_t lastModified;

	bool isValid() {
		return name.size() > 0 && path.size() > 0;
	}

	friend std::ostream& operator << (std::ostream& out, const ProjectData& c) {
		char name[64] = { 0 };
		memcpy(name, c.name.data(), c.name.size());
		out.write(name, 64*sizeof(char));
		char path[1024] = { 0 };
		memcpy(path, c.path.data(), c.path.size());
		out.write(path, sizeof(char)*1024);
		char thumbnailPath[1024] = { 0 };
		if (c.thumbNail)
			memcpy(thumbnailPath, c.thumbNail->GetName().data(), c.thumbNail->GetName().size());
		out.write(thumbnailPath, 1024*sizeof(char));
		char time[256] = { 0 };
		memcpy(time, c.time.data(), c.time.size());
		out.write(time, sizeof(time));
		out.write((char*)&c.lastModified, sizeof(uint64_t));
		return out;
	};

	friend std::istream& operator>>(std::istream& in, ProjectData& c) {
		char name[64] = { 0 };
		in.read(name, 64*sizeof(char));
		char path[1024] = { 0 };
		in.read(path, 1024*sizeof(char));
		char texture[1024] = { 0 };
		in.read(texture, 1024*sizeof(char));
		char time[256] = { 0 };
		in.read(time, 256*sizeof(char));
		uint64_t lastModified = 0;
		in.read((char*)&lastModified, sizeof(uint64_t));
		if (strlen(texture) > 0)
			c.thumbNail = GEngine::Texture2D::Create(texture);
		c.name = name;
		c.path = path;
		c.time = time;
		c.lastModified = lastModified;
		return in;
	}

	inline bool operator==(const ProjectData& other) {
		return other.path == this->path;
	}


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
	std::string m_lastProjectDir;

	char m_search_char[512] = { 0 };
	bool m_createProjectModal = false;
	bool m_confirmDeleteModal = false;

	GEngine::Ref<GEngine::Texture2D> searchIcon;
	GEngine::Ref<GEngine::Texture2D> checkerBoardIcon;
	GEngine::Ref<GEngine::Texture2D> folderIcon;

	void Search();
	void Sort(int i );
	void SaveProjects();
	void LoadProjects();
	void ImportProject(const std::string& path);
	void CreateNewProjectDialog();
	void CreateDeleteConfirmationDialog();
    void CreateProject(ProjectData* d);
	void OpenProject(const std::string& path);
	void DeleteProject(const std::string& path);
	ProjectData* GetProjectDataFromPath(const std::string& path);
};

