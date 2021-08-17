#include "ProjectSelectLayer.h"
#include "GenerateProject.h"
#include "Editor/EditorLayer.h"

#include "Shared/ImpulseEditor.h"

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
		searchIcon = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/searchIcon160x160.png", TEXTUREFLAGS_Mag_Linear| TEXTUREFLAGS_Min_Linear);
		folderIcon = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/folderIcon172x172.png");
		projectSelectionIcon = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/package160x160.png");


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

		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y - 2.5f });
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4(ImGuiCol_Text));
		if (ImGui::InputTextWithHint("##ProjectSearch", "Search", m_search_char, sizeof(m_search_char))) {
			if (strlen(m_search_char) > 0) {
				Search();
			}
		}
		ImGui::PopStyleColor();

		//ImGui::SameLine(0, 0);
		//pos = ImGui::GetCursorPos();
		//ImGui::SetCursorPos({ pos.x - 20.f, pos.y + 5.f });
		//ImGui::Image((ImTextureID)(uintptr_t)searchIcon->GetRendererID(), { 16,16 }, { 0,1 }, { 1,0 });
		ImGui::EndChild();

		ImGui::BeginChild("Projects", { (float)GEngine::Application::GetWindowWidth() * .85f,(float)GEngine::Application::GetWindowHeight() * .85f }, true);
		for (const LocalProject& proj : strlen(m_search_char) > 0 ? m_projectData_Searched : m_projectData) {
			const ProjectData& p = proj.data;
			std::string s = "##" + p.name;
			if (ImGui::Selectable(s.c_str(), selectedProject == (p.path + "/" + p.name), 0, { 0.f, 60.f })) {
				selectedProject = p.path + "/" + p.name;
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				if (OpenProject(selectedProject)) {
					ImGui::EndChild();
					ImGui::End();
					return;
				}
			}

			ImGui::SameLine();
			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos({ pos.x, pos.y + 5.f });
			GEngine::Ref<GEngine::Texture2D> tex = p.thumbNail;
			if (tex == nullptr) {
				tex = projectSelectionIcon;
			}
			ImGui::Image((ImTextureID)(uintptr_t)tex->GetRendererID(), { 50.f,50.f }, { 0,1 }, { 1,0 });
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
            if (OpenProject(selectedProject)) {
                ImGui::EndChild();

                ImGui::End();
                return;
            }
		}

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("Show", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			ShowProject(selectedProject);
		}
		
		if (selectedProject.size() > 0 && GetProjectDataFromPath(selectedProject)->data.languages & (uint32_t)Project::ProjectDataLanguages::NATIVE) {
			ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
			if (ImGui::Button("Generate", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
				LocalProject* proj = GetProjectDataFromPath(selectedProject);
				m_generateFlags = proj->generateFlags;
				m_generateBuild = proj->buildFlags;
				m_generatePlatform = proj->platformFlags;
				m_generateModal = true;
				ImGui::OpenPopup("Generate Project");
				//ShowProject(selectedProject);
			}
		}
		CreateGenerateDialog();

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		if (ImGui::Button("Remove", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			RemoveProject(selectedProject);
		}

		ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
		ImGui::PushStyleColor(ImGuiCol_Text, { 1.f,0.f,0.f,1.f });
		if (ImGui::Button("Delete", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
			m_confirmDeleteModal = true;
			m_deleteFail = false;
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
		for (const LocalProject& p : m_projectData) {
			std::string name = p.data.name;
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
		std::vector<LocalProject>& d = m_projectData_Searched.size() > 0 ? m_projectData_Searched : m_projectData;
		switch (i) {
		default:
		case 0: {
			std::sort(d.begin(), d.end(), [](const LocalProject& lhs, const LocalProject& rhs) {
				return lhs.data.lastModified > rhs.data.lastModified;
				});
			break;
		}
		case 1: {
			std::sort(d.begin(), d.end(), [](const LocalProject& lhs, const LocalProject& rhs) {
				return lhs.data.name.compare(rhs.data.name) < 0;
				});
			break;
		}
		case 2: {
			std::sort(d.begin(), d.end(), [](const LocalProject& lhs, const LocalProject& rhs) {
				return lhs.data.path.compare(rhs.data.path) < 0;
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
			char path[1008] = { 0 };
			memcpy(path, m_lastProjectDir.data(), sizeof(char) * m_lastProjectDir.size());
			stream.write(path, 1008 * sizeof(char));
			memset(path, 0, 1008 * sizeof(char));
			uint64_t prjCount = m_projectData.size();
			stream.write((char*)&prjCount, sizeof(uint64_t));
			for (LocalProject& proj : m_projectData) {
				const ProjectData& p = proj.data;
				std::string _path = p.path + "/" + p.name + "/" + p.name + ".proj";
				memcpy(path, _path.data(), _path.size() * sizeof(char));
				stream.write(path, 1008 * sizeof(char));
				stream.write((const char*)&proj.generateFlags, sizeof(proj.generateFlags));
				stream.write((const char*)&proj.buildFlags, sizeof(proj.buildFlags));
				stream.write((const char*)&proj.platformFlags, sizeof(proj.platformFlags));
				stream.write((const char*)proj.compilerDir, sizeof(proj.compilerDir));
				memset(path, 0, 1008 * sizeof(char));
			}
			stream.close();
		}
	}

	void ProjectSelectLayer::LoadProjects()
	{
		std::string saveLoc = GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor/.projects";

		std::ifstream stream(saveLoc, std::ios::in | std::ios::binary);

		if (stream.is_open()) {
			char path[1008] = { 0 };
			stream.read(path, sizeof(char) * 1008);
			m_lastProjectDir = path;
			memset(path, 0, 1008 * sizeof(char));
			uint64_t prjCount = 0;
			stream.read((char*)&prjCount, sizeof(uint64_t));
			LocalProject proj;
			for (int i = 0; i < prjCount; i++) {
				stream.read(path, sizeof(char) * 1008);
				stream.read((char*)&proj.generateFlags, sizeof(proj.generateFlags));
				stream.read((char*)&proj.buildFlags, sizeof(proj.buildFlags));
				stream.read((char*)&proj.platformFlags, sizeof(proj.platformFlags));
				stream.read((char*)proj.compilerDir, sizeof(proj.compilerDir));
				std::ifstream in(path, std::ios::in | std::ios::binary);
				in >> proj.data;
				in.close();
				if (proj.data.path.size() > 0)
					m_projectData.push_back(std::move(proj));
				memset(path, 0, 1008 * sizeof(char));
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
			if (ImGui::ImageButton((ImTextureID)(uintptr_t)folderIcon->GetRendererID(), { 20.f,20.f }, { .0f,1.f }, { 1.f, 0.f })) {
				std::string fileLoc;
				GEngine::FileSystem::OpenFileDialog({}, fileLoc, m_newProjectLocation, true);
				if (fileLoc.size() > 0) {
					m_newProjectLocation = fileLoc;

				}
			}
			ImGui::RadioButton("Scripting", (int*)&m_newProjectLanguage, (uint32_t)ProjectDataLanguages::SCRIPTING);
			ImGui::SameLine();
			ImGui::RadioButton("Native", (int*)&m_newProjectLanguage, (uint32_t)ProjectDataLanguages::NATIVE);

			if (ImGui::Button("Create", ImVec2(120, 0))) {
				m_newProjectError = "";
				for (auto& p : m_projectData) {
					if ((p.data.path + "/" + p.data.name) == (m_newProjectLocation + "/" + m_newProjectName)) {
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
#ifdef GE_PLATFORM_WINDOWS
					m_generatePlatform = (uint32_t)Generation::PlatformFlags::WINDOWS;
#elif GE_PLATFORM_MACOSX
					m_generatePlatform = (uint32_t)Generation::PlatformFlags::MACOSX;
#else
					m_generatePlatform = (uint32_t)Generation::PlatformFlags::LINUX;
#endif

					m_generateFlags = Generation::GenerateProject::GetDefaultGenerationFlags(static_cast<Generation::PlatformFlags>(m_generatePlatform));
					m_generateBuild = (uint32_t)Generation::GenerateProject::GetDefaultProjectType(static_cast<Generation::PlatformFlags>(m_generatePlatform));
                    
					m_projectData.push_back({ ProjectData(m_newProjectName, nullptr, m_newProjectLocation, time, (uint64_t)_t, (uint32_t)m_newProjectLanguage), m_generatePlatform,m_generateFlags,m_generateBuild,  GEngine::Utility::sys::default_msbuild() });
					Sort(m_sortType);
					Search();
					selectedProject = m_newProjectLocation + "/" + m_newProjectName;
					CreateProject(&GetProjectDataFromPath(selectedProject)->data);
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

	void ProjectSelectLayer::CreateGenerateDialog()
	{
#ifdef GE_EDITOR
		if (m_generateModal) ImGui::SetNextWindowSize({ 400,0 });
		if (ImGui::BeginPopupModal("Generate Project", &m_generateModal, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			ImGui::Text("Platform: ");
			ImGui::SameLine();
			

			auto& d = Generation::GenerateProject::GetPlatformData();
			Generation::PlatformStructure platformStruct = d[static_cast<Generation::PlatformFlags>(m_generatePlatform)];

			if (ImGui::BeginCombo("##Platform", Generation::GenerateProject::PlatformFlagToStr(static_cast<Generation::PlatformFlags>(m_generatePlatform)).c_str())) {

				for (const auto& p : d) {

					bool is_selected = (uint32_t)p.first == m_generatePlatform;
					if (ImGui::Selectable(Generation::GenerateProject::PlatformFlagToStr(p.first).c_str(), is_selected)) {
						m_generatePlatform = (uint32_t)p.first;
						m_generateFlags = Generation::GenerateProject::GetDefaultGenerationFlags(static_cast<Generation::PlatformFlags>(m_generatePlatform));
						platformStruct = d[static_cast<Generation::PlatformFlags>(m_generatePlatform)];
						m_generateBuild = (uint32_t)Generation::GenerateProject::GetDefaultProjectType(static_cast<Generation::PlatformFlags>(m_generatePlatform));
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();

					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Project Type: ");
			ImGui::SameLine();

			if (ImGui::BeginCombo("##Project", Generation::GenerateProject::ProjectFlagToStr(static_cast<Generation::ProjectTypeFlags>(m_generateBuild)).c_str())) {

				for (const auto& p : platformStruct.projectFlags) {

					bool is_selected = (uint32_t)p == m_generateBuild;
					if (ImGui::Selectable(Generation::GenerateProject::ProjectFlagToStr(p).c_str(), is_selected)) {
						m_generateBuild = (uint32_t)p;
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (platformStruct.generationFlags.size() > 1) {


				ImGui::BeginChild("GenerateFlagsLeft", { ImGui::GetWindowWidth() / 2,  (platformStruct.generationFlags.size() / 2.f) * 40.f }, false);

				for (size_t i = platformStruct.generationFlags.size() <= 1 ? 2 : platformStruct.generationFlags.size() / 2; i < platformStruct.generationFlags.size(); i++) {
					ImGui::CheckboxFlags(Generation::GenerateProject::GenerateFlagStr(platformStruct.generationFlags[i]).c_str(), (unsigned int*)&m_generateFlags, (uint32_t)platformStruct.generationFlags[i]);
				}

				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("GenerateFlagsRight", { ImGui::GetWindowWidth() / 2, (platformStruct.generationFlags.size() / 2.f) * 40.f}, false);

				for (int i = 0; i < platformStruct.generationFlags.size() / 2; i++) {
					ImGui::CheckboxFlags(Generation::GenerateProject::GenerateFlagStr(platformStruct.generationFlags[i]).c_str(), (unsigned int*)&m_generateFlags, (uint32_t)platformStruct.generationFlags[i]);
				}

				ImGui::EndChild();	
				
			}
			else {
				ImGui::CheckboxFlags(Generation::GenerateProject::GenerateFlagStr(platformStruct.generationFlags[0]).c_str(), (unsigned int*)&m_generateFlags, (uint32_t)platformStruct.generationFlags[0]);
			}

			ImGui::Separator();
			if (ImGui::Button("Generate")) {
				GenerateProject();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

#endif
	}

	void ProjectSelectLayer::CreateDeleteConfirmationDialog()
	{
#ifdef GE_EDITOR
		if (m_confirmDeleteModal) ImGui::SetNextWindowSize({ 400, 0 });
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

			ProjectData* d = &GetProjectDataFromPath(selectedProject)->data;
			ImGui::TextColored({ 1.f,0.f,0.f,1.f }, (std::string("Are you sure you want to delete:")).c_str());
			std::string _path = d->path + "/" + d->name;
			std::replace(_path.begin(), _path.end(), '\\', '/');
			ImGui::Text((d->name + "\n" + _path + "/").c_str());
			ImGui::TextColored({ 1.f,1.f,0.f,1.f }, (std::string("WARNING:\nDeleting this project will destroy its entire directory\n")).c_str());
			ImGui::Separator();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.f - 40);
			if (ImGui::Button("Yes")) {
				if (DeleteProject(selectedProject)) {
					selectedProject = "";
					m_confirmDeleteModal = false;
					m_deleteFail = false;
					ImGui::CloseCurrentPopup();
				}
				else {
					m_deleteFail = true;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("No")) {
				m_confirmDeleteModal = false;
				ImGui::CloseCurrentPopup();
			}
			if (m_deleteFail) {
				ImGui::TextColored({ 1.f,0.f,0.f,1.f }, "Permission Denied.");
			}
			ImGui::EndPopup();
		}
#endif
	}


	bool ProjectSelectLayer::OpenProject(const std::string& path)
	{
		LocalProject* proj = GetProjectDataFromPath(selectedProject);
		ProjectData* d = &proj->data;
#if 0
		if (d->isNative()) {
			GEngine::ScriptApi::SetBuild_Native(path + "/" + d->name + "/" + d->name + "/NativeScripts/", "NativeScripts");
			GEngine::ScriptApi::SetMake_Native("", "", [this]() {
				return GenerateProject();
				});
			GEngine::ScriptApi::SetDLLDir_Native(path + "/" + d->name + "/Bin/Release-" + Generation::GenerateProject::PlatformFlagToDebugStr(static_cast<Generation::PlatformFlags>(m_generatePlatform)) + "-x86_64/"+d->name+"/");
			GEngine::ScriptApi::OutputDir_Native(path + "/" + d->name + "/" + d->name + "/NativeScripts/Generated/");
			GEngine::ScriptApi::SetRelativePath_Native("../Scripts/");
			GEngine::ScriptApi::Load(path + "/" + d->name + "/" + d->name + "/NativeScripts/Scripts/", ".h");
			//GEngine::ScriptApi::GetReflector_Native()->CallStaticFunction<void>("ExampleScript", "TestFunction");
		}
#endif

		Editor::EditorLayer* layer = Editor::EditorLayer::Create(proj);
		if (layer) {
			GEngine::Application::GetApp()->PushLayer(layer);
			GEngine::Application::GetApp()->PopLayer(this);
			delete this;// change this maybe idk
		}
		return true;
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
					m_projectData.push_back(LocalProject( pData, 1,0,0, GEngine::Utility::sys::default_msbuild() ));
					Search();
					Sort(0);
				}
				SaveProjects();
			}
		}
	}

	bool ProjectSelectLayer::DeleteProject(const std::string& path)
	{
		std::vector<LocalProject>::iterator it = m_projectData.begin();
		std::string dir;
		while (it != m_projectData.end()) {
			if ((it->data.path + "/" + it->data.name) == path) {
				dir = it->data.path + "/" + it->data.name + "/";
				break;
			}
			it++;
		}
		bool canDel = GEngine::FileSystem::RemoveAllFolders(dir);
		if (!canDel) {
			return false;
		}
		selectedProject = "";
		m_projectData.erase(it);
		if (dir.size() > 0) {
			SaveProjects();
			Search();
		}

		return true;
	}

	void ProjectSelectLayer::RemoveProject(const std::string& path)
	{
		std::vector<LocalProject>::iterator it = m_projectData.begin();
		std::string dir;
		while (it != m_projectData.end()) {
			if ((it->data.path + "/" + it->data.name) == path) {
				dir = it->data.path + "/" + it->data.name + "/";
				m_projectData.erase(it);
				selectedProject = "";
				break;
			}
			it++;
		}
	}

	void ProjectSelectLayer::CreateProject(ProjectData* d) {

		for (const std::string& s : create_project_dirs)
			GEngine::FileSystem::CreateDirectories(d->path + "/" + d->name + "/" + d->name + "/" + d->name + "/" + s);


		GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/AndroidStudio.zip", d->path + "/" + d->name + "/" + d->name+"/AndroidStudio");
		GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/Tools.zip", d->path + "/" + d->name + "/Tools");
		if (d->isNative()) {
			GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/Generate.zip", d->path + "/" + d->name + "/" + d->name + "/Generate");
			GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/BuildTarget.zip", d->path + "/" + d->name + "/" + d->name);
			GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/vendor.zip", d->path + "/" + d->name + "/vendor");
			GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/Scripts.zip", d->path + "/" + d->name + "/" + d->name + "/" + d->name + "/NativeScripts");
			GEngine::FileSystem::ExtractZip("Content/EditorContent/Archives/Shared.zip", d->path + "/" + d->name + "/" + d->name + "/" + d->name + "/Source/Shared");
			GEngine::FileSystem::Copy(d->path + "/" + d->name + "/" + d->name + "/BuildTarget.lua", d->path + "/" + d->name + "/premake5.lua", false, false);
		}

        
		std::string filePath = d->path + "/" + d->name + "/" + d->name + ".proj";
		std::ofstream out(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
		out << *d;
		out.close();

		GEngine::FileSystem::Copy("Data/EngineContent.pak", d->path + "/" + d->name + "/" + d->name + "/" + d->name + "/Data/EngineContent.pak", true, false);

		SaveProjects();
		/*
		if (d->isNative()) {
#ifndef GE_PLATFORM_WINDOWS
			std::string permission = "chmod +x \"" + selectedProject + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\"";
			GE_CORE_DEBUG("CMD: {0}", permission);
			GE_CORE_DEBUG("RESULT: {0}", GEngine::Utility::sys::exec_command(cmd));
#endif

			std::string cmd = "\"" + selectedProject + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\" " + GE_PLATFORM_MAKE + " --os=" + GE_PLATFORM_OS + " --engine-source=" + GEngine::FileSystem::GetParentExecuteableDir(4) + " --hot-reload --package --build-engine --target-name=" + d->name;
#ifdef GE_PLATFORM_WINDOWS
			cmd += " --build-openal";
#endif
			std::replace(cmd.begin(), cmd.end(), '/', '\\');
			GE_CORE_DEBUG("CMD: {0}", cmd);
			std::string re = GEngine::Utility::sys::exec_command(cmd);
			GE_CORE_DEBUG("RESULT: {0}", re);
		}
		*/
	}

	bool ProjectSelectLayer::GenerateProject(bool retry)
	{
		ProjectData* d = &GetProjectDataFromPath(selectedProject)->data;
        std::string path = GEngine::FileSystem::GetParentExecuteableDir(4);
        
		std::string flags = Generation::GenerateProject::GenerateCommand(static_cast<Generation::PlatformFlags>(m_generatePlatform), static_cast<Generation::ProjectTypeFlags>(m_generateBuild), m_generateFlags);
		std::string cmd = "\"" + selectedProject + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\" "+flags + " --engine-source=" +  path + " --target-name=\""+d->name+"\"";
#ifdef GE_PLATFORM_WINDOWS
		std::replace(cmd.begin(), cmd.end(), '/', '\\');
#endif
		GE_CORE_DEBUG("CMD {0}", cmd);
		std::string re = GEngine::Utility::sys::exec_command(cmd);
#ifndef GE_PLATFORM_WINDOWS
        if (retry) {
            if (re.find("Permission denied") != std::string::npos) {
                std::string chmod = "chmod +x \"" + selectedProject + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\"";
                GE_CORE_DEBUG("CMD {0}", chmod);
                GEngine::Utility::sys::exec_command(chmod);
                return GenerateProject(false);
            }
        }
#endif
		LocalProject* proj = GetProjectDataFromPath(selectedProject);
		proj->buildFlags = m_generateBuild;
		proj->generateFlags = m_generateFlags;
		proj->platformFlags = m_generatePlatform;
		SaveProjects();
		return true;
		GE_CORE_DEBUG("RESULT: {0}", re);	
	}

	void ProjectSelectLayer::ShowProject(const std::string& path)
	{
#ifdef GE_PLATFORM_WINDOWS
		ProjectData* d = &GetProjectDataFromPath(selectedProject)->data;
		GE_CORE_WARN("FLAGS: {0}, {1}, {2}", GetProjectDataFromPath(selectedProject)->buildFlags, GetProjectDataFromPath(selectedProject)->generateFlags, GetProjectDataFromPath(selectedProject)->platformFlags);
		std::string cmd = "explorer /select,\"" + selectedProject + "/" + d->name + ".proj\"";
		std::replace(cmd.begin() + 15, cmd.end(), '/', '\\');
		GEngine::Utility::sys::exec_command(cmd);

#endif
#ifdef GE_PLATFORM_LINUX
		GEngine::Utility::sys::exec_command("xdg-open " + selectedProject);
#endif
#ifdef GE_PLATFORM_MACOSX
		GEngine::Utility::sys::exec_command("open " + selectedProject);
#endif

	}

	LocalProject* ProjectSelectLayer::GetProjectDataFromPath(const std::string& path)
	{
		std::vector<LocalProject>::iterator it = m_projectData.begin();
		while (it != m_projectData.end()) {
			if ((it->data.path + "/" + it->data.name) == path) {
				return &*it; // 1:12
			}
			it++;
		}
		return nullptr;
	}

}

