#include "ProjectSelectLayer.h"
#ifdef GE_EDITOR
#include "imgui/imgui_internal.h"
#endif

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
	searchIcon = GEngine::Texture2D::Create("Content/Textures/searchIcon64x64.png", 192);

	for (int i = 0; i < 20; i++) {
		GEngine::Ref<GEngine::Texture2D> tex = GEngine::Texture2D::Create("D:/Documents/Dev/ImpulsiveEngine/ImpulsiveEngine/ImpulseEditor/ImpulseEditor/Content/Textures/ImpulsiveGamesLogo.png", 192);
		if (!tex) {
			tex = GEngine::Texture2D::Create("Content/Textures/Checkerboard.png", 192);
		}

		stringstream transTime;
		std::time_t _t = std::time(0)-i*5;   // get time now
		transTime << std::put_time(std::localtime(&_t), "%c");
		std::string time = transTime.str();
		transTime.clear();
		
		ProjectData p = { "Test" + std::to_string(i), tex, "D:/Documents/Dev/ImpulsiveEngine/ImpulsiveEngine/ImpulseEditor/" + std::to_string(i), time, (uint64_t)_t};
		m_projectData.push_back(std::move(p));
	}

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

	ImGui::BeginChild("Sorting", { (float)GEngine::Application::GetWindowWidth()*.25f,30.f }, false);
	
	ImVec2 pos = ImGui::GetCursorPos();
	ImGui::SetCursorPos({ pos.x, pos.y + 5.f });
	ImGui::Text("Sort:");
	ImGui::SameLine();
	pos = ImGui::GetCursorPos();
	ImGui::SetCursorPos({ pos.x, pos.y - 2.5f });
	if (ImGui::BeginCombo("##SortType", m_sortType.c_str())) {
		for (int i = 0; i < m_sortTypes.size(); i++)
		{
			bool is_selected = m_sortType == m_sortTypes[i];
			if (ImGui::Selectable(m_sortTypes[i].c_str(), is_selected)) {
				m_sortType = m_sortTypes[i];
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
	
	ImGui::BeginChild("Projects", { (float)GEngine::Application::GetWindowWidth()*.85f,(float)GEngine::Application::GetWindowHeight()*.85f }, true);
	for (const ProjectData& p : strlen(m_search_char) > 0 ? m_projectData_Searched : m_projectData) {
		std::string s = "##" + p.name;
		if (ImGui::Selectable(s.c_str(), selectedProject == p.path, 0, { 0.f, 60.f })) {
			selectedProject = p.path;
		}
		ImGui::SameLine();
		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y + 5.f });
		ImGui::Image((ImTextureID)p.thumbNail->GetRendererID(), { 50.f,50.f }, { 0,1 }, { 1,0 });
		ImGui::SameLine();
		pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y - 2.5f });
		std::string title = p.name + "\n" + p.path+"\nLast Modified: "+p.time;
		ImGui::Text(title.c_str());
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Buttons", { (float)GEngine::Application::GetWindowWidth() * .125f, (float)GEngine::Application::GetWindowHeight() * .85f }, true);
	
	ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
	if (ImGui::Button("New", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
		GE_LOG_DEBUG("Button Pressed!");
	}
	ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
	if (ImGui::Button("Import", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
		GE_LOG_DEBUG("Button Pressed!");
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
		GE_LOG_DEBUG("Open!");
	}

	if (prjSelec) {
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
		prjSelec = false;
	}
	ImGui::SetCursorPosX((float)GEngine::Application::GetWindowWidth() * .125f / 2.f - (float)GEngine::Application::GetWindowWidth() * .125f * .75f / 2.f);
	if (!selectedProject.size()) {
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		prjSelec = true;
	}
	if (ImGui::Button("Delete", { (float)GEngine::Application::GetWindowWidth() * .125f * .75f,0 })) {
		std::vector<ProjectData>::iterator it = m_projectData.begin();
		while (it != m_projectData.end()) {
			if (it->path == selectedProject) {
				m_projectData.erase(it);
				selectedProject = "";
				break;
			}
			it++;
		}
		Search();
	}
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
	switch (i) {
	default:
	case 0: {
		std::sort(m_projectData.begin(), m_projectData.end(), [](const ProjectData& lhs, const ProjectData& rhs) {
			return lhs.lastModified < rhs.lastModified;
			});
		break;
	}
	case 1: {
		std::sort(m_projectData.begin(), m_projectData.end(), [](const ProjectData& lhs, const ProjectData& rhs) {
			return lhs.name.compare(rhs.name) < 0;
			});
		break;
	}
	case 2: {
		std::sort(m_projectData.begin(), m_projectData.end(), [](const ProjectData& lhs, const ProjectData& rhs) {
			return lhs.path.compare(rhs.path) < 0;
			});
		break;
	}
	}
}
