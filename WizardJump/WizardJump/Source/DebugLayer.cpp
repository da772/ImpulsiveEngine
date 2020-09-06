#include "DebugLayer.h"

bool DebugLayer::showLog = true;

bool handleResize = false;

DebugLayer::DebugLayer() : Layer("DebugLayer")
{

}



void DebugLayer::OnImGuiRender()
{
	if (DebugLayer::showLog)
		ShowDock(&DebugLayer::showLog);
	if (DebugLayer::showLog)
		GEngine::Log::GetImGuiLog()->Draw("Console Log", &DebugLayer::showLog);

	if (GEngine::Application::DebugTools()) {
		CreateViewPort();
		CreateSceneHierarchy();
	}

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
	if (event.GetEventType() == GEngine::EventType::WindowResize) {
		//handleResize = true;
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

void DebugLayer::CreateSceneHierarchy()
{

	ImGui::Begin("Scene Hierarchy"); {

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Entities"))
		{

			std::unordered_map<uint64_t, GEngine::Ref<GEngine::Entity>> entities = GEngine::SceneManager::GetCurrentScene()->GetEntities();
			for ( const std::pair<uint64_t, GEngine::Ref<GEngine::Entity>>& e : entities )
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
				if (ImGui::TreeNode((void*)(intptr_t)e.first, "%s (%ju)", typeid(*e.second.get()).name(),e.first))
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.75, .75, .75, 1));
					std::unordered_map<uint64_t, GEngine::Ref<GEngine::Component>> components = e.second->GetComponents();

					for (const std::pair<uint64_t, GEngine::Ref<GEngine::Component>>& c : components) {
						std::string compName = typeid(*c.second.get()).name();
						if (ImGui::TreeNode((void*)(intptr_t)c.first, "%s (%ju)", typeid(*c.second.get()).name(), c.first)) {

							if (compName == "class GEngine::Transform") {
								float v[3];
								GEngine::Ref<GEngine::Transform> transform = dynamic_pointer_cast<GEngine::Transform>(c.second);

								if (!transform) return;

								v[0] = transform->GetPosition().x;
								v[1] = transform->GetPosition().y;
								v[2] = transform->GetPosition().z;
								if (ImGui::InputFloat3("Position", v, 4)) {
									if (v[0] != transform->GetPosition().x || v[1] != transform->GetPosition().y || v[2] != transform->GetPosition().z) {
										e.second->SetEntityPosition({ v[0], v[1], v[2] });
									}
								}



							}



							ImGui::TreePop();
						}
					}

					ImGui::PopStyleColor(1);
					ImGui::TreePop();
				}
				ImGui::PopStyleColor(1);
			}
			
			ImGui::TreePop();
		}
		


		
	}
	ImGui::End();

}

glm::vec2 lastFrameSize;
glm::vec2 finalSize = { 0, 0 };
glm::vec2 originalSize = { 0,0 };

glm::vec2 scaleRatio(int maxWidth, int maxHeight, int imgWidth, int imgHeight) {


	// calc
	float widthRatio = (float)maxWidth / (float)imgWidth,
		heightRatio = (float)maxHeight / (float)imgHeight;
	float bestRatio = min(widthRatio, heightRatio);

	// output
	int newWidth = (float)imgWidth * bestRatio,
		newHeight = (float)imgHeight * bestRatio;
	return { newWidth, newHeight };
}




void DebugLayer::CreateViewPort()
{
	if (originalSize.x == 0) {
		originalSize = { GEngine::Application::GetApp()->m_viewPortWidth,GEngine::Application::GetApp()->m_viewPortHeight };
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("ViewPort");
	GEngine::Application::SetInputEnabled(ImGui::IsWindowFocused());
	ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
	glm::vec2 sz = { viewPortSize.x, viewPortSize.y };
	glm::vec2 csz = { GEngine::RenderPipeline::GetFrameBuffer()->GetTexture()->GetWidth(), GEngine::RenderPipeline::GetFrameBuffer()->GetTexture()->GetHeight() };
	if (lastFrameSize != sz || handleResize) {
		
		finalSize = scaleRatio(sz.x, sz.y, originalSize.x, originalSize.y);
		GEngine::Application::GetApp()->m_viewPortWidth = finalSize.x;
		GEngine::Application::GetApp()->m_viewPortHeight = finalSize.y;
		//GEngine::RenderPipeline::GetFrameBuffer()->UpdateSize(originalSize.x, originalSize.y);
		lastFrameSize = sz;
		/*
		handleResize = false;
		*/
	}


	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - finalSize.x) * .5f , ImGui::GetCursorPosY() });
	GEngine::Application::SetViewPortOffset({ ImGui::GetCursorPosX() + ImGui::GetWindowPos().x, ImGui::GetCursorPosY() + ImGui::GetWindowPos().y });
	ImGui::Image((void*)(intptr_t)GEngine::RenderPipeline::GetFrameBuffer()->GetTexture()->GetRendererID(), { finalSize.x, finalSize.y }, { 0,1 }, { 1,0 });
	ImGui::End();
	ImGui::PopStyleVar();
}

