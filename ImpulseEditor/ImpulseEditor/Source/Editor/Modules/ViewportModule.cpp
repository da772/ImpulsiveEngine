#include "ViewportModule.h"
#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorApplicationEvents.h"
#include "imgui/imgui_internal.h"

using namespace GEngine;

namespace Editor {
	ViewportModule::ViewportModule(const std::string& pipeline) : m_pipeline(pipeline)
	{
		m_textures["playButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/play172x172.png");
		m_textures["pauseButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/pause172x172.png");
		m_textures["stopButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/stop172x172.png");
		m_textures["resumeButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/resume172x172.png");
		m_textures["fastForwardButton"] = GEngine::Texture2D::Create("Content/Textures/Icons/fastForward172x172.png");
	}

	ViewportModule::~ViewportModule()
	{

	}

	void ViewportModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		if (originalSize.x == 0) {
			originalSize = { GEngine::Application::GetApp()->m_viewPortWidth,GEngine::Application::GetApp()->m_viewPortHeight };
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin(name.c_str(), 0, flags);
		
		ImGui::BeginChild("ViewPortControls", { 0, imageButtonSize });
		ImGui::BeginChild("dropdown", {250, 0});
		if (ImGui::BeginCombo("##AspectRatio", aspectRatio.c_str())) {

			if (ImGui::Selectable("(16:9) 1920x1080")) {
				GEngine::Application::GetApp()->m_viewPortWidth = 1920;
				GEngine::Application::GetApp()->m_viewPortHeight = 1080;
				originalSize = { GEngine::Application::GetApp()->m_viewPortWidth,GEngine::Application::GetApp()->m_viewPortHeight };
				aspectRatio = "1920x1080";
				handleResize = true;

			}
			if (ImGui::Selectable("(9:16) 1080x1920")) {
				GEngine::Application::GetApp()->m_viewPortWidth = 1080;
				GEngine::Application::GetApp()->m_viewPortHeight = 1920;
				originalSize = { GEngine::Application::GetApp()->m_viewPortWidth,GEngine::Application::GetApp()->m_viewPortHeight };
				aspectRatio = "1080x1920";
				handleResize = true;
			}
			if (ImGui::Selectable("Custom")) {
				GE_CORE_DEBUG("SELECTED Custom");
				originalSize = { GEngine::Application::GetApp()->m_viewPortWidth,GEngine::Application::GetApp()->m_viewPortHeight };
				aspectRatio = "Custom";
				handleResize = true;
			}

			ImGui::EndCombo();
		}

		ImGui::EndChild();

		ImGui::SameLine();

		if (GEngine::SceneManager::HasBegun()) {
			
			if (GEngine::Application::IsGamePaused()) {
				ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f - imageButtonSize * 2 , ImGui::GetCursorPosY()});
				if (ControlButtons("resumeButton")) {
					GEngine::Application::ResumeGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationResumeEvent>();
				}
				ImGui::SameLine();
				if (ControlButtons("fastForwardButton")) {
					GEngine::Application::ResumeGame();
					GEngine::ThreadPool::AddEndFrameFunction([]() {GEngine::Application::PauseGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPauseEvent>(); });
				}
				ImGui::SameLine();
			}
			else {
				ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f - imageButtonSize, ImGui::GetCursorPosY() });
				if (ControlButtons("pauseButton")) {
					GEngine::Application::PauseGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPauseEvent>();
				}
				ImGui::SameLine();
			}
			
			if (ControlButtons("stopButton")) {
				ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f , ImGui::GetCursorPosY() });
				const std ::string name =  GEngine::SceneManager::GetCurrentScene()->GetId();
				if (GEngine::Application::IsGamePaused()) {
					GEngine::Application::ResumeGame();
				}
				GEngine::SceneManager::SetCurrentScene(name.c_str(), false);
				EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationStopEvent>();
			}
			
		}
		else {
			ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f , ImGui::GetCursorPosY() });
			if (ControlButtons("playButton")) {
				GEngine::SceneManager::Begin();
				EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPlayEvent>();
			}
			
		}
		ImGui::EndChild();

		ImGui::BeginChild("ViewPortImpl");

		GEngine::Application::SetInputEnabled(ImGui::IsWindowFocused());
		ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
		Vector2<float> sz = { viewPortSize.x, viewPortSize.y };
		GEngine::Ref<GEngine::RenderPipeline> pipeline = GEngine::Renderer::GetPipeline(m_pipeline.c_str());
		Vector2<uint32_t> csz = { pipeline->GetFrameBuffer()->GetTexture()->GetWidth(), pipeline->GetFrameBuffer()->GetTexture()->GetHeight() };
		if (lastFrameSize != sz || handleResize) {

			finalSize = scaleRatio(sz.x, sz.y, originalSize.x, originalSize.y);
			GEngine::Application::GetApp()->m_viewPortWidth = finalSize.x;
			GEngine::Application::GetApp()->m_viewPortHeight = finalSize.y;
			//pipeline->GetFrameBuffer()->UpdateSize(1080, 1920);
			lastFrameSize = sz;

			/*
			handleResize = false;
			*/
		}

		ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - finalSize.x) * .5f , ImGui::GetCursorPosY() });
		GEngine::Application::SetViewPortOffset({ ImGui::GetCursorPosX() + ImGui::GetWindowPos().x, ImGui::GetCursorPosY() + ImGui::GetWindowPos().y });
		ImGui::Image((void*)(intptr_t)pipeline->GetFrameBuffer()->GetTexture()->GetRendererID(), { (float)finalSize.x,(float)finalSize.y }, { 0,1 }, { 1,0 });
		ImGui::EndChild();

		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
		ImGui::PopStyleVar();
	}

	GEngine::Vector2<uint32_t> ViewportModule::scaleRatio(int maxWidth, int maxHeight, int imgWidth, int imgHeight)
	{
		// calc
		float widthRatio = (float)maxWidth / (float)imgWidth,
			heightRatio = (float)maxHeight / (float)imgHeight;
		float bestRatio = std::min(widthRatio, heightRatio);

		// output
		int newWidth = (float)imgWidth * bestRatio,
			newHeight = (float)imgHeight * bestRatio;
		return { (uint32_t)newWidth, (uint32_t)newHeight };
	}

	bool ViewportModule::ControlButtons(const std::string& s)
	{
		ImGui::Image((ImTextureID)m_textures[s]->GetRendererID(), { imageButtonSize, imageButtonSize }, { 0,1 }, { 1,0 });
		if (ImGui::IsItemHovered()) {
			return ImGui::IsMouseClicked(0);
		}
		return false;

	}

}
