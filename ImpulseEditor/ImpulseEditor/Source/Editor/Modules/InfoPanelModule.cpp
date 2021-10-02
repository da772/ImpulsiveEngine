#ifdef GE_EDITOR
#include "InfoPanelModule.h"

namespace Editor {

	InfoPanelModule::InfoPanelModule()
	{
		m_textures["warning"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/warning172x172.png");
		m_textures["error"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/error172x172.png");
		m_textures["info"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/info172x172.png");
		m_textures["bug"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/bug172x172.png");
	}

	InfoPanelModule::~InfoPanelModule()
	{

	}

	void InfoPanelModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (true)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos({ viewport->Pos.x,viewport->Pos.y + viewport->Size.y - offset });
			ImGui::SetNextWindowSize({ viewport->Size.x , size });
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.

		//window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(name.c_str(), 0, window_flags | flags);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + fontOffset);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + fontOffset);
		float fontSize = ImGui::GetFontSize();
		
		

		int level = GEngine::Log::GetImGuiLog()->GetLastLevel();

		if (level == 0 || level == 2) {
			ImGui::PushStyleColor(ImGuiCol_Text, { 255.f,255.f,255.f,255.f });
			ImGui::Image((ImTextureID)(uintptr_t)m_textures["info"]->GetRendererID(), { fontSize, fontSize }, { 0,1 }, { 1,0 });
		}
		else if (level == 1) {
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.f,230.f,255.f,255.f });
			ImGui::Image((ImTextureID)(uintptr_t)m_textures["bug"]->GetRendererID(), { fontSize, fontSize }, { 0,1 }, { 1,0 });
		}
		else if (level == 3) {
			ImGui::PushStyleColor(ImGuiCol_Text, { 255.f,255.f,0.f,255.f });
			ImGui::Image((ImTextureID)(uintptr_t)m_textures["warning"]->GetRendererID(), { fontSize, fontSize }, { 0,1 }, { 1,0 });
		}
		else if (level == 4) {
			ImGui::PushStyleColor(ImGuiCol_Text, { 255.f,0.f,0,255.f });
			ImGui::Image((ImTextureID)(uintptr_t)m_textures["error"]->GetRendererID(), { fontSize, fontSize }, { 0,1 }, { 1,0 });
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX());
		
		ImGui::Text(GEngine::Log::GetImGuiLog()->GetLastLog().c_str());
		ImGui::PopStyleColor();

		ImGui::End();

		ImGui::PopStyleVar(3);
	}

}
#endif