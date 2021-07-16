#include "ToolbarModule.h"
#include "ReloadModule.h"

#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorApplicationEvents.h"

namespace Editor {

	ToolbarModule::ToolbarModule(ReloadModule* reloadModule)  : m_reloadModule(reloadModule)
	{
		m_textures["playButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/play172x172.png");
		m_textures["pauseButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/pause172x172.png");
		m_textures["stopButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/stop172x172.png");
		m_textures["resumeButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/resume172x172.png");
		m_textures["fastForwardButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/fastForward172x172.png");
		m_textures["loadingButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/loadingIcon172x172.png");
		m_textures["handButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/hand172x172.png");
		m_textures["scaleButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/scale172x172.png");
		m_textures["rotateButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/rotate172x172.png");
		m_textures["moveButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/drag172x172.png");
	}

	ToolbarModule::~ToolbarModule()
	{

	}

	void ToolbarModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (true)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize({ viewport->Size.x , 50.f });
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.

		//window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(name.c_str(), 0, window_flags | flags | ImGuiWindowFlags_NoScrollbar);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15.f);
		ImGui::SetCursorPosY( ImGui::GetCursorPosY() + imageBorderSize - imageButtonSize + yOffset );
		if (ControlButtons("handButton")) {


		}
		ImGui::SameLine(0.f, 15.f);
		
		if (ControlButtons("moveButton")) {

		}
		ImGui::SameLine(0.f, 15.f);
		
		if (ControlButtons("rotateButton")) {

		}
		ImGui::SameLine(0.f, 15.f);
		if (ControlButtons("scaleButton")) {

		}
		ImGui::SameLine(0.f, 15.5f);
		if (GEngine::SceneManager::HasBegun()) {

			if (GEngine::Application::IsGamePaused()) {

				ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f - imageButtonSize * 2 , ImGui::GetCursorPosY() + yOffset });
				if (ControlButtons("resumeButton")) {
					GEngine::Application::ResumeGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationResumeEvent>();
				}
				ImGui::SameLine(0.f, 15.f);
				if (ControlButtons("fastForwardButton")) {
					GEngine::Application::ResumeGame();
					GEngine::ThreadPool::AddEndFrameFunction([]() {GEngine::Application::PauseGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPauseEvent>(); });
				}
				ImGui::SameLine(0.f, 15.f);
			}
			else {
				ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f - imageButtonSize, ImGui::GetCursorPosY() + yOffset });
				if (ControlButtons("pauseButton")) {
					GEngine::Application::PauseGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPauseEvent>();
				}
				ImGui::SameLine(0.f, 15.f);
			}

			if (ControlButtons("stopButton")) {
				ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f , ImGui::GetCursorPosY()+yOffset });
				const std::string name = GEngine::SceneManager::GetCurrentScene()->GetId();
				if (GEngine::Application::IsGamePaused()) {
					GEngine::Application::ResumeGame();
				}
				GEngine::SceneManager::SetCurrentScene(name.c_str(), false);
				EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationStopEvent>();
			}

		}
		else {
			ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f , ImGui::GetCursorPosY() + yOffset });
			if (m_reloadModule->IsReloading()) {
				ControlButtons("loadingButton");
			}
			else {
				if (ControlButtons("playButton")) {
					GEngine::SceneManager::Begin();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPlayEvent>();
				}
			}

		}

		ImGui::End();

		ImGui::PopStyleVar(3);


	}

	bool ToolbarModule::ControlButtons(const std::string& s)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		bool isHovered = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect({ pos.x-imageBorderSize*.25f, pos.y - (imageBorderSize-imageButtonSize)/2.f }, { pos.x + imageBorderSize + imageBorderSize * .25f, pos.y + imageBorderSize });
		ImGui::GetWindowDrawList()->AddRectFilled({ pos.x-imageBorderSize * .25f, pos.y - (imageBorderSize - imageButtonSize) / 2.f }, { pos.x + imageBorderSize + imageBorderSize * .25f, pos.y + imageBorderSize}, ImGui::GetColorU32(ImGui::GetStyleColorVec4(!isHovered ? ImGuiCol_Button : ImGuiCol_ButtonHovered)), 0.f);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (imageBorderSize - imageButtonSize) / 2.f);
		ImGui::Image((ImTextureID)(uintptr_t)m_textures[s]->GetRendererID(), { imageButtonSize, imageButtonSize}, { 0,1 }, { 1,0 });
		if (isHovered) {
			return ImGui::IsMouseClicked(0);
		}
		return false;
	}

}