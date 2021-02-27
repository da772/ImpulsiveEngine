#include "DebugLayer.h"
#include "imgui/imgui_internal.h"


using namespace GEngine;

bool DebugLayer::showLog = true;

bool handleResize = false;

bool DebugLayer::showScene = true;
bool DebugLayer::showGraphicsDebugger = true;


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
			if (showScene) CreateSceneHierarchy();
			if (showGraphicsDebugger) CreateGraphicsDebuggger();
		}
}


static void RefreshAssets() {
	GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Content",
		GEngine::FileSystem::FilePath("Data/Content.pak"), false);
	GEngine::FileSystem::LoadPak("Data/Content.pak");
	GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"),
		GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Data/Content.pak", false);
	GE_LOG_DEBUG("Refreshing Pak File");
}

static void ReCreateGraphics() {
	GEngine::Application::UnloadGraphics();
	GEngine::Application::ReloadGraphics();
}


void DebugLayer::OnEvent(GEngine::Event& event)
{
	if (event.GetEventType() == GEngine::EventType::KeyPressed) {
		GEngine::KeyPressedEvent& e = (GEngine::KeyPressedEvent&)event;
		if (e.GetKeyCode() == GE_KEY_TILDE) {
			DebugLayer::showLog = !DebugLayer::showLog;
		}
		if (e.GetKeyCode() == GE_KEY_F11) {
			if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT_CONTROL)) {
				RefreshAssets();
				ReCreateGraphics();
			}
		}

		if (e.GetKeyCode() == GE_KEY_F12) {
			if (GEngine::Application::IsGamePaused()) {
				GEngine::Application::ResumeGame();
			}
			else {
				GEngine::Application::PauseGame();
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
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
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
	ImGui::Begin("My DockSpace", NULL, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		if (!ImGui::DockBuilderGetNode(dockspace_id)) {
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_CentralNode);


			ImGuiID dock_main_id = dockspace_id;
			
			ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.25f, nullptr, &dock_main_id);
			ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);
			ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);
			ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, .25f, nullptr, &dock_main_id);
			ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

			ImGui::DockBuilderDockWindow("ViewPort", ImGui::DockBuilderGetCentralNode(dock_up_id)->ID);
			//ImGui::DockBuilderDockWindow("Actions", dock_up_id);
			ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_left_id);
			ImGui::DockBuilderDockWindow("Inspector", dock_right_id);
			//ImGui::DockBuilderDockWindow("Graphics Debugger", dock_left_id);
			ImGui::DockBuilderDockWindow("Console Log", dock_down_id);
			//ImGui::DockBuilderDockWindow("Project", dock_down_right_id);

			ImGuiDockNode* node = ImGui::DockBuilderGetNode(ImGui::DockBuilderGetCentralNode(dockspace_id)->ID);
			node->LocalFlags |=  ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
			node = ImGui::DockBuilderGetNode(dock_down_id);
			node->LocalFlags |=  ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
			node = ImGui::DockBuilderGetNode(dock_right_id);
			node->LocalFlags |=  ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
			node = ImGui::DockBuilderGetNode(dock_left_id);
			node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;

			ImGui::DockBuilderFinish(dock_main_id);

		}



		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Refresh Assets", "CTRL-R", false)) {
				RefreshAssets();
				ReCreateGraphics();
			}
			
			if (ImGui::MenuItem("Hide Console", "", DebugLayer::showLog == 0))
				DebugLayer::showLog = !DebugLayer::showLog;
			ImGui::EndMenu();
		}



		if (ImGui::BeginMenu("Edit")) {
			ImGui::Text("Test");


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {

			ImGui::Text("Test");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();



	}
	


	ImGui::End();
}

GEngine::Ref<GEngine::GameObject> hashSelected = nullptr;
GEngine::Ref<GEngine::GameObject> compSelected = nullptr;


bool b_component = false;

void DebugLayer::CreateSceneHierarchy()
{

	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Scene Hierarchy"); {

		
		if (ImGui::Button("Add")) {

		}

	

		if (hashSelected != nullptr) {
			ImGui::SameLine();
			if (ImGui::Button("Destroy")) {
				
				if (b_component) {
					GEngine::Ref<GEngine::Component> __c = static_pointer_cast<GEngine::Component>(hashSelected);
					__c->Destroy();
				}
				else {
					GEngine::Ref<GEngine::Entity> __e = static_pointer_cast<GEngine::Entity>(hashSelected);
					__e->Destroy();
				}

				hashSelected = nullptr;
				b_component = false;

			}
		}
		
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Entities"))
		{
			std::unordered_map<uint64_t, GEngine::Ref<GEngine::Entity>> entities = GEngine::SceneManager::GetCurrentScene()->GetEntities();
			for ( const std::pair<uint64_t, GEngine::Ref<GEngine::Entity>>& e : entities )
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
				ImGuiTreeNodeFlags entity_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				if (hashSelected == e.second) {
					entity_base_flags |= ImGuiTreeNodeFlags_Selected;
				}
				bool show_e = ImGui::TreeNodeEx((void*)(intptr_t)e.first, entity_base_flags, "%s : %s (%ju)", e.second->m_tag.c_str(), typeid(*e.second.get()).name(), e.first);
				if (ImGui::IsItemClicked()) {
					b_component = false;
					hashSelected = e.second;
				}
					
				if (show_e)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.75, .75, .75, 1));
					std::unordered_map<uint64_t, GEngine::Ref<GEngine::Component>> components = e.second->GetComponents();

					

					for (const std::pair<uint64_t, GEngine::Ref<GEngine::Component>>& c : components) {
						std::string compName = typeid(*c.second.get()).name();
						ImGuiTreeNodeFlags component_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
						if (hashSelected == c.second) {
							component_base_flags |= ImGuiTreeNodeFlags_Selected;
						}
						bool show_c = ImGui::TreeNodeEx((void*)(intptr_t)c.first, component_base_flags, "%s : %s (%ju)", c.second->m_tag.c_str(), typeid(*c.second.get()).name(), c.first);
						if (ImGui::IsItemClicked()) {
							b_component = true;
							hashSelected = c.second;
						}
						if (show_c) {
							
							if (compName == "class GEngine::Transform") {
								float v[3];
								GEngine::Ref<GEngine::Transform> transform = dynamic_pointer_cast<GEngine::Transform>(c.second);

								if (!transform) return;

								v[0] = transform->GetPosition().x;
								v[1] = transform->GetPosition().y;
								v[2] = transform->GetPosition().z;
								ImGui::Text("Position: ");
								ImGui::SameLine();
								if (ImGui::InputFloat3("", v,  4)) {
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

Vector2<float> lastFrameSize;
Vector2<uint32_t> finalSize = { 0, 0 };
Vector2<int> originalSize = { 0,0 };

static Vector2<uint32_t> scaleRatio(int maxWidth, int maxHeight, int imgWidth, int imgHeight) {


	// calc
	float widthRatio = (float)maxWidth / (float)imgWidth,
		heightRatio = (float)maxHeight / (float)imgHeight;
	float bestRatio = min(widthRatio, heightRatio);

	// output
	int newWidth = (float)imgWidth * bestRatio,
		newHeight = (float)imgHeight * bestRatio;
	return { (uint32_t)newWidth, (uint32_t)newHeight };
}




void DebugLayer::CreateViewPort()
{
	if (originalSize.x == 0) {
		originalSize = {GEngine::Application::GetApp()->m_viewPortWidth,GEngine::Application::GetApp()->m_viewPortHeight };
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("ViewPort");



	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x) * .5f , ImGui::GetCursorPosY() });

	if (GEngine::Application::IsGamePaused()) {

		if (ImGui::Button(">")) {
			GEngine::Application::ResumeGame();
		}
		ImGui::SameLine();
		if (ImGui::Button(">>")) {
			GEngine::Application::ResumeGame();
			GEngine::ThreadPool::AddEndFrameFunction([]() {GEngine::Application::PauseGame(); });
		}
	}
	else {
		if (ImGui::Button("||")) {
			GEngine::Application::PauseGame();
		}
	}
	

	const char* pipelineId = "viewport";
	GEngine::Application::SetInputEnabled(ImGui::IsWindowFocused());
	ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
	Vector2<float> sz = { viewPortSize.x, viewPortSize.y };
	GEngine::Ref<GEngine::RenderPipeline> pipeline = GEngine::Renderer::GetPipeline(pipelineId);
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
	ImGui::Image((void*)(intptr_t)pipeline->GetFrameBuffer()->GetTexture()->GetRendererID(), { (float)finalSize.x,(float) finalSize.y }, { 0,1 }, { 1,0 });

	
	ImGui::End();
	ImGui::PopStyleVar();
}

std::string textureSelected;

void DebugLayer::CreateGraphicsDebuggger()
{
	ImGui::Begin("Graphics Debugger");

	ImGui::Text("Batch Count: %d", GEngine::Batch::GetBatchCount());

	auto& textures = GEngine::Texture2D::GetLoadedTextures();
	if (ImGui::TreeNodeEx("Textures Loaded", 0,  "Textures Loaded: %d", textures.size())) {
		for (auto& a : textures) {
			ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (textureSelected == a.first)
				base_flags |= ImGuiTreeNodeFlags_Selected;
			
			bool show_c = ImGui::TreeNodeEx((void*)(intptr_t)&a.first, base_flags, "%s", a.first.c_str());
			if (ImGui::IsItemClicked()) {
				textureSelected = a.first;
			}
			if (show_c) {
				if (!a.second.expired()) {
					if (ImGui::ImageButton((ImTextureID)a.second.lock()->GetRendererID(), { 50,50 }, { 0,1 }, { 1,0 })) {
						ImGui::OpenPopup("texture_pop_up");
					}
					if (ImGui::BeginPopup("texture_pop_up", ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Image((ImTextureID)a.second.lock()->GetRendererID(), {(float) a.second.lock()->GetWidth(),(float)a.second.lock()->GetHeight()}, { 0,1 }, { 1,0 });
						ImGui::EndPopup();
					}
				}
				ImGui::TreePop();
			}
			
		}
		ImGui::TreePop();
	}



	ImGui::End();
}


