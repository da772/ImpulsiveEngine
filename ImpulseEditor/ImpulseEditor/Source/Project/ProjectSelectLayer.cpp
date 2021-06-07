#include "ProjectSelectLayer.h"
#ifdef GE_EDITOR
#include "imgui/imgui_internal.h"
#endif

namespace Project {

	static const std::vector<std::string> create_project_dirs = { "Content", "Data" };

	ProjectSelectLayer::ProjectSelectLayer(const std::string& name) : Layer(name)
	{

	}

	ProjectSelectLayer::~ProjectSelectLayer()
	{

	}

	void ProjectSelectLayer::Begin()
	{

	}

	void ProjectSelectLayer::OnAttach()
	{
		searchIcon = GEngine::Texture2D::Create("Content/Textures/searchIcon172x172.png");
		folderIcon = GEngine::Texture2D::Create("Content/Textures/folderIcon172x172.png");
		checkerBoardIcon = GEngine::Texture2D::Create("Content/Textures/Checkerboard.png");

		LoadProjects();
		Sort(0);

	}

	void ProjectSelectLayer::OnDetach()
	{
	}

	void ProjectSelectLayer::OnUpdate(GEngine::Timestep timeStep)
	{
	}

	void ProjectSelectLayer::OnEvent(GEngine::Event& event)
	{
	}

	void ProjectSelectLayer::OnImGuiRender()
	{
#ifdef GE_EDITOR

		//ImGui::ShowDemoWindow();

		ImGui::SetNextWindowSize({ (float)GEngine::Application::GetWindowWidth(), (float)GEngine::Application::GetWindowHeight() });
		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::Begin("Project Selector", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Select a Project:");

		ImGui::Separator();

		ImGui::BeginChild("Sorting", { (float)GEngine::Application::GetWindowWidth() * .25f,30.f }, false);

		ImVec2 pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y + 5.f });
		ImGui::Text("Sort:");
		ImGui::SameLine();
		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y - 2.5f });
		if (ImGui::BeginCombo("##SortType", m_sortTypes[m_sortType].c_str())) {
			for (int i = 0; i < m_sortTypes.size(); i++)
			{
				bool is_selected = m_sortType == i;
				if (ImGui::Selectable(m_sortTypes[i].c_str(), is_selected)) {
					m_sortType = i;
					Sort(i);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndCombo();
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild("Searching", { (float)GEngine::Application::GetWindowWidth() * .3f,35.f }, false);
		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y + 5.f });
		ImGui::Text("Search:");
		ImGui::SameLine();

		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y - 2.5f });
		if (ImGui::InputText("##ProjectSearch", m_search_char, sizeof(m_search_char))) {
			if (strlen(m_search_char) > 0) {
				Search();
			}
		}
		ImGui::SameLine(0, 0);
		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x - 20.f, pos.y + 5.f });
		ImGui::Image((ImTextureID)searchIcon->GetRendererID(), { 16,16 }, { 0,1 }, { 1,0 });
		ImGui::EndChild();

		ImGui::BeginChild("Projects", { (float)GEngine::Application::GetWindowWidth() * .85f,(float)GEngine::Application::GetWindowHeight() * .85f }, true);
		for (const ProjectData& p : strlen(m_search_char) > 0 ? m_projectData_Searched : m_projectData) {
			std::string s = "##" + p.name;
			if (ImGui::Selectable(s.c_str(), selectedProject == (p.path + "/" + p.name), 0, { 0.f, 60.f })) {
				selectedProject = p.path + "/" + p.name;
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				OpenProject(selectedProject);
			}

			ImGui::SameLine();
			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos({ pos.x, pos.y + 5.f });
			GEngine::Ref<GEngine::Texture2D> tex = p.thumbNail;
			if (tex == nullptr) {
				tex = checkerBoardIcon;
			}
			ImGui::Image((ImTextureID)tex->GetRendererID(), { 50.f,50.f }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos({ pos.x, pos.y - 2.5f });
			std::string _path = p.path;
			//std::replace(_path.begin(), _path.end(), '\\', '/');
			std::string title = p.name + "\n" + _path + "/" + p.name + "\nLast Modified: " + p.time;
			ImGui::Text(title.c_str());
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Buttons", { (float)GEngine::Application::GetWindowWidth() * .125f, (float)GEngine::Application::GetWindowHeight() * .85f }, true);

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("New", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			m_newProjectName = m_defaultProjectName;
			m_createProjectModal = true;
			m_newProjectError = "";
			m_newProjectLocation = m_lastProjectDir.size() > 0 ? m_lastProjectDir : GEngine::FileSystem::GetParentExecuteableDir(0);
			m_newProjectLanguage = (uint32_t)ProjectDataLanguages::SCRIPTING;
			ImGui::OpenPopup("Create New Project");
		}
		CreateNewProjectDialog();
		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("Import", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			std::string ret;
			GEngine::FileSystem::OpenFileDialog({ {"Project", "proj"} }, ret);
			ImportProject(ret);
		}
		ImGui::Separator();
		bool prjSelec = false;
		if (!selectedProject.size()) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			prjSelec = true;
		}
		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("Open", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			OpenProject(selectedProject);
		}

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("Show", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			ShowProject(selectedProject);
		}

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("Remove", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			RemoveProject(selectedProject);
		}

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		ImGui::PushStyleColor(ImGuiCol_Text, { 1.f,0.f,0.f,1.f });
		if (ImGui::Button("Delete", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			m_confirmDeleteModal = true;
			ImGui::OpenPopup("Delete Project?");
		}
		ImGui::PopStyleColor();

		CreateDeleteConfirmationDialog();
		if (prjSelec) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
			prjSelec = false;
		}

		ImGui::EndChild();



		ImGui::End();

#endif	
	}

	void ProjectSelectLayer::OnDraw()
	{
	}

	void ProjectSelectLayer::End()
	{
	}

	void ProjectSelectLayer::Search()
	{
		m_projectData_Searched.clear();
		if (strlen(m_search_char) <= 0) return;
		for (const ProjectData& p : m_projectData) {
			std::string name = p.name;
			std::string s = m_search_char;
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			if (name.find(s) != std::string::npos) {
				m_projectData_Searched.push_back(p);
			}
		}
	}

	void ProjectSelectLayer::Sort(int i)
	{
		std::vector<ProjectData>& d = m_projectData_Searched.size() > 0 ? m_projectData_Searched : m_projectData;
		switch (i) {
		default:
		case 0: {
			std::sort(d.begin(), d.end(), [](const ProjectData& lhs, const ProjectData& rhs) {
				return lhs.lastModified > rhs.lastModified;
				});
			break;
		}
		case 1: {
			std::sort(d.begin(), d.end(), [](const ProjectData& lhs, const ProjectData& rhs) {
				return lhs.name.compare(rhs.name) < 0;
				});
			break;
		}
		case 2: {
			std::sort(d.begin(), d.end(), [](const ProjectData& lhs, const ProjectData& rhs) {
				return lhs.path.compare(rhs.path) < 0;
				});
			break;
		}
		}
	}

	void ProjectSelectLayer::SaveProjects()
	{
		std::string file = GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor/.projects";
		std::ofstream stream;
		stream.open(file, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!stream.is_open()) {
			GEngine::FileSystem::CreateDirectories(GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor");
			stream = std::ofstream(file, std::ios::out | std::ios::binary | std::ios::trunc);
		}

		if (stream.is_open()) {
			char path[1024] = { 0 };
			memcpy(path, m_lastProjectDir.data(), sizeof(char) * m_lastProjectDir.size());
			stream.write(path, 1024 * sizeof(char));
			memset(path, 0, 1024 * sizeof(char));
			uint64_t prjCount = m_projectData.size();
			stream.write((char*)&prjCount, sizeof(uint64_t));
			for (ProjectData& p : m_projectData) {
				std::string _path = p.path + "/" + p.name + "/" + p.name + ".proj";
				memcpy(path, _path.data(), _path.size() * sizeof(char));
				stream.write(path, 1024 * sizeof(char));
				memset(path, 0, 1024 * sizeof(char));
			}
			stream.close();
		}
	}

	void ProjectSelectLayer::LoadProjects()
	{
		std::string saveLoc = GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor/.projects";

		std::ifstream stream(saveLoc, std::ios::in | std::ios::binary);

		if (stream.is_open()) {
			char path[1024] = { 0 };
			stream.read(path, sizeof(char) * 1024);
			m_lastProjectDir = path;
			memset(path, 0, 1024 * sizeof(char));
			uint64_t prjCount = 0;
			stream.read((char*)&prjCount, sizeof(uint64_t));

			for (int i = 0; i < prjCount; i++) {
				stream.read(path, sizeof(char) * 1024);
				ProjectData pData;
				std::ifstream in(path, std::ios::in | std::ios::binary);
				in >> pData;
				in.close();
				if (pData.path.size() > 0)
					m_projectData.push_back(std::move(pData));
				memset(path, 0, 1024 * sizeof(char));
			}

			stream.close();
		}

	}

	void ProjectSelectLayer::CreateNewProjectDialog()
	{
#ifdef GE_EDITOR
		if (m_createProjectModal) ImGui::SetNextWindowSize({ 400, 0 });
		if (ImGui::BeginPopupModal("Create New Project", &m_createProjectModal, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			if (!ImGui::IsWindowHovered()) {
				if (ImGui::IsMouseClicked(0)) {
					m_createProjectModal = false;
					ImGui::CloseCurrentPopup();
				}
			}

			if (GEngine::Input::IsKeyPressed(GE_KEY_ESCAPE)) {
				m_confirmDeleteModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::Text("Project Name:");
			char nameBuf[255] = { 0 };
			ImGui::PushItemWidth(200);
			memcpy(nameBuf, &m_newProjectName[0], m_newProjectName.size());
			if (ImGui::InputText("##newProjectName", nameBuf, 255)) {
				m_newProjectName = nameBuf;
			}
			ImGui::PopItemWidth();
			ImGui::Text("Project Path:");

			char pathBuf[1024] = { 0 };
			ImGui::PushItemWidth(350);
			memcpy(pathBuf, &m_newProjectLocation[0], m_newProjectLocation.size());
			if (ImGui::InputText("##newProjectPath", pathBuf, 1024)) {
				m_newProjectLocation = pathBuf;
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			float xPos = ImGui::GetCursorPosX();
			ImGui::SetCursorPosX(xPos - 7.f);
			if (ImGui::ImageButton((ImTextureID)folderIcon->GetRendererID(), { 20.f,20.f }, { .0f,1.f }, { 1.f, 0.f })) {
				std::string fileLoc;
				GEngine::FileSystem::OpenFileDialog({}, fileLoc, m_newProjectLocation, true);
				if (fileLoc.size() > 0) {
					m_newProjectLocation = fileLoc;

				}
			}
			ImGui::RadioButton("Scripting", (int*)&m_newProjectLanguage, (uint32_t)ProjectDataLanguages::SCRIPTING);
			ImGui::SameLine();
			ImGui::RadioButton("Native", (int*)&m_newProjectLanguage, (uint32_t)ProjectDataLanguages::NATIVE);
			ImGui::SameLine();
			ImGui::RadioButton("Both", (int*)&m_newProjectLanguage, (uint32_t)ProjectDataLanguages::SCRIPTING | (uint32_t)(ProjectDataLanguages::NATIVE));

			if (ImGui::Button("Create", ImVec2(120, 0))) {
				m_newProjectError = "";
				for (auto& p : m_projectData) {
					if ((p.path + "/" + p.name) == (m_newProjectLocation + "/" + m_newProjectName)) {
						m_newProjectError = "Project already created in that directory";
					}
				}
				if (m_newProjectError.size() <= 0) {
					std::stringstream transTime;
					std::time_t _t = std::time(0);   // get time now
					transTime << std::put_time(std::localtime(&_t), "%c");
					std::string time = transTime.str();
					transTime.clear();
					std::replace(m_newProjectLocation.begin(), m_newProjectLocation.end(), '\\', '/');
					m_lastProjectDir = m_newProjectLocation;
					m_projectData.push_back({ m_newProjectName, nullptr, m_newProjectLocation, time, (uint64_t)_t, m_newProjectLanguage});
					Sort(m_sortType);
					Search();
					selectedProject = m_newProjectLocation + "/" + m_newProjectName;
					CreateProject(GetProjectDataFromPath(selectedProject));
					m_createProjectModal = false;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { m_createProjectModal = false; ImGui::CloseCurrentPopup(); }
			if (m_newProjectError.size() > 0) {
				ImGui::TextColored({ 1.f,0.f,0.f,1.f }, "Error: %s", m_newProjectError.c_str());
			}
			ImGui::EndPopup();
		}
#endif
		//std::string str;
		//GEngine::FileSystem::OpenFileDialog({}, str, true);
		//GE_CORE_DEBUG("FOLDER OPEN: {0}", str);
	}

	void ProjectSelectLayer::CreateDeleteConfirmationDialog()
	{
#ifdef GE_EDITOR
		if (m_confirmDeleteModal) ImGui::SetNextWindowSize({ 400, 200 });
		if (ImGui::BeginPopupModal("Delete Project?", &m_confirmDeleteModal, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			if (!ImGui::IsWindowHovered()) {
				if (ImGui::IsMouseClicked(0)) {
					m_confirmDeleteModal = false;
					ImGui::CloseCurrentPopup();
				}
			}

			if (GEngine::Input::IsKeyPressed(GE_KEY_ESCAPE)) {
				m_confirmDeleteModal = false;
				ImGui::CloseCurrentPopup();
			}

			ProjectData* d = GetProjectDataFromPath(selectedProject);
			ImGui::TextColored({ 1.f,0.f,0.f,1.f }, (std::string("Are you sure you want to delete:")).c_str());
			std::string _path = d->path + "/" + d->name;
			std::replace(_path.begin(), _path.end(), '\\', '/');
			ImGui::Text((d->name + "\n" + _path + "/").c_str());
			ImGui::TextColored({ 1.f,1.f,0.f,1.f }, (std::string("WARNING:\nDeleting this project will destroy its entire directory\n")).c_str());
			if (ImGui::Button("Yes")) {
				DeleteProject(selectedProject);
				selectedProject = "";
				m_confirmDeleteModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No")) {
				m_confirmDeleteModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
#endif
	}

	void ProjectSelectLayer::OpenProject(const std::string& path)
	{
		GE_CORE_DEBUG("@TODO OPEN PROJECT: {0}", path);
	}

	void ProjectSelectLayer::ImportProject(const std::string& path) {
		if (path.size() > 0) {
			ProjectData pData;
			std::ifstream in(path, std::ios::in | std::ios::binary);
			in >> pData;
			in.close();

			std::string p = pData.path + "/" + pData.name + "/" + pData.name + ".proj";
			std::ifstream test(p, std::ios::in);

			if (test) {
				test.close();
				if (std::find(m_projectData.begin(), m_projectData.end(), pData) == m_projectData.end()) {
					m_projectData.push_back(std::move(pData));
					Search();
					Sort(0);
				}
				SaveProjects();
			}
		}
	}

	void ProjectSelectLayer::DeleteProject(const std::string& path)
	{
		std::vector<ProjectData>::iterator it = m_projectData.begin();
		std::string dir;
		while (it != m_projectData.end()) {
			if ((it->path + "/" + it->name) == path) {
				dir = it->path + "/" + it->name + "/";
				m_projectData.erase(it);
				selectedProject = "";
				break;
			}
			it++;
		}
		if (dir.size() > 0) {
			GEngine::FileSystem::RemoveAllFolders(dir);
			SaveProjects();
			Search();

		}
	}

	void ProjectSelectLayer::RemoveProject(const std::string& path)
	{
		std::vector<ProjectData>::iterator it = m_projectData.begin();
		std::string dir;
		while (it != m_projectData.end()) {
			if ((it->path + "/" + it->name) == path) {
				dir = it->path + "/" + it->name + "/";
				m_projectData.erase(it);
				selectedProject = "";
				break;
			}
			it++;
		}
	}

	void ProjectSelectLayer::CreateProject(ProjectData* d) {
		GE_CORE_DEBUG("@TODO CREATE PROJECT");

		for (const std::string& s : create_project_dirs)
			GEngine::FileSystem::CreateDirectories(d->path + "/" + d->name + "/" + d->name + "/" + d->name + "/" + s);

		GEngine::FileSystem::ExtractZip("Content/Archives/AndroidStudio.zip", d->path + "/" + d->name + "/" + d->name+"/AndroidStudio");
		GEngine::FileSystem::ExtractZip("Content/Archives/Generate.zip", d->path + "/" + d->name + "/" + d->name + "/Generate");
		GEngine::FileSystem::ExtractZip("Content/Archives/BuildTarget.zip", d->path + "/" + d->name + "/" + d->name);
		GEngine::FileSystem::ExtractZip("Content/Archives/vendor.zip", d->path + "/" + d->name+"/vendor");
		GEngine::FileSystem::ExtractZip("Content/Archives/Tools.zip", d->path + "/" + d->name + "/Tools");
		GEngine::FileSystem::ExtractZip("Content/Archives/Scripts.zip", d->path + "/" + d->name + "/" + d->name + "/" + d->name+"/NativeScripts");
		GEngine::FileSystem::ExtractZip("Content/Archives/Source.zip", d->path + "/" + d->name + "/" + d->name + "/" + d->name);

		GEngine::FileSystem::Copy(d->path + "/" + d->name + "/" + d->name + "/BuildTarget.lua", d->path + "/" + d->name + "/premake5.lua", false, false);

		std::string filePath = d->path + "/" + d->name + "/" + d->name + ".proj";
		std::ofstream out(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
		out << *d;
		out.close();

		GEngine::FileSystem::Copy("Data/EngineContent.pak", d->path + "/" + d->name + "/" + d->name + "/" + d->name + "/Data/EngineContent.pak", true, false);

		SaveProjects();

		std::string cmd = "\"" + selectedProject + "/" + d->name + "/Generate/GenerateProject.bat\" vs2019 --os=windows --engine-source=" +GEngine::FileSystem::GetParentExecuteableDir(4)+ " --hot-reload --build-openal --package --build-engine --target-name=" + d->name;
		std::replace(cmd.begin(), cmd.end(), '/', '\\');
		GE_CORE_DEBUG("CMD: {0}", cmd);
		std::string re = GEngine::Utility::sys::exec_command(cmd);
		GE_CORE_DEBUG("RESULT: {0}", re);

	}

	void ProjectSelectLayer::ShowProject(const std::string& path)
	{
#ifdef GE_PLATFORM_WINDOWS
		ProjectData* d = GetProjectDataFromPath(selectedProject);
		std::string cmd = "explorer /select,\"" + selectedProject + "/" + d->name + ".proj\"";
		std::replace(cmd.begin() + 15, cmd.end(), '/', '\\');
		GEngine::Utility::sys::exec_command(cmd);
		cmd = "\"" + selectedProject + "/" + d->name + "/Generate/GenerateProject_Windows.bat\" vs2019 --os=windows --package --target-name=" + d->name;
		std::replace(cmd.begin(), cmd.end(), '/', '\\');
		GE_CORE_DEBUG("CMD: {0}", cmd);
		std::string re = GEngine::Utility::sys::exec_command(cmd);
		GE_CORE_DEBUG("RESULT: {0}", re);

#endif
#ifdef GE_PLATFORM_LINUX
		GEngine::Utility::sys::exec_command("xdg-open " + selectedProject);
#endif
#ifdef GE_PLATFORM_MACOSX
		GEngine::Utility::sys::exec_command("open " + selectedProject);
#endif

	}

	ProjectData* ProjectSelectLayer::GetProjectDataFromPath(const std::string& path)
	{
		std::vector<ProjectData>::iterator it = m_projectData.begin();
		while (it != m_projectData.end()) {
			if ((it->path + "/" + it->name) == path) {
				return &*it; // ???
			}
			it++;
		}
		return nullptr;
	}

}

