#include "DebugLayer.h"

bool DebugLayer::showLog = false;

DebugLayer::DebugLayer() : Layer("DebugLayer")
{

}

void DebugLayer::OnImGuiRender()
{
	//ImGui::PushFont(font2);
	if (DebugLayer::showLog)
		ShowDock(&DebugLayer::showLog);
	if (DebugLayer::showLog)
		GEngine::Log::GetImGuiLog()->Draw("Console Log", &DebugLayer::showLog);

	//ImGui::PopFont();
}

void DebugLayer::OnEvent(GEngine::Event& event)
{
	if (event.GetEventType() == GEngine::EventType::KeyPressed) {
		GEngine::KeyPressedEvent& e = (GEngine::KeyPressedEvent&)event;
		if (e.GetKeyCode() == GE_KEY_TILDE) {
			DebugLayer::showLog = !DebugLayer::showLog;
		}
		if (e.GetKeyCode() == GE_KEY_R) {
			if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT_CONTROL)) {
				GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Content",
					GEngine::FileSystem::FilePath("Data/Content.pak"), false);
				GEngine::FileSystem::LoadPak("Data/Content.pak");
				GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"),
					GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Data/Content.pak", false);
				GE_LOG_DEBUG("Refreshing Pak File");
				GEngine::SceneManager::SetCurrentScene("splashScreen");
			}
		}
	}	
}

void DebugLayer::ShowDock(bool p_open)
{
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	else
	{

	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Editor"))
		{

			if (ImGui::MenuItem("Refresh Assets", "CTRL-R", false)) {
				GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Content",
					GEngine::FileSystem::FilePath("Data/Content.pak"), false);
				GEngine::FileSystem::LoadPak("Data/Content.pak");
				GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"),
					GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Data/Content.pak", false);
				GE_LOG_DEBUG("Refreshing Pak File");
				GEngine::SceneManager::SetCurrentScene("splashScreen");
			}
			
			if (ImGui::MenuItem("Hide Console", "", DebugLayer::showLog == 0))
				DebugLayer::showLog = !DebugLayer::showLog;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

