#include "ViewportModule.h"
#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorApplicationEvents.h"
#include "imgui/imgui_internal.h"
#include "Editor/Modules/SerializerModule.h"


using namespace GEngine;

namespace Editor {
	ViewportModule::ViewportModule(const std::string& pipeline, ReloadModule* reloadModule, bool gameView, GEngine::CameraController* cam, EditorEventType* tools, SerializerModule* serliazer) : m_pipeline(pipeline), m_reloadModule(reloadModule), gameView(gameView),
		m_cameraController(cam), editorTools(tools), m_serializer(serliazer)
	{
		updates = !gameView;
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
		if (!ImGui::Begin(name.c_str(), 0, flags)) {
			ImGui::PopStyleVar();
			ImGui::End();
			return;
		}
		
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

			if (handleResize && m_serializer)
			{
				m_serializer->Load(m_serializer->SerializeCurrentScene());
			}

			ImGui::EndCombo();
		}

		ImGui::EndChild();
		ImGui::EndChild();

		ImGui::BeginChild("ViewPortImpl", {0,0}, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		isFocused = ImGui::IsWindowFocused();
		if ((gameView && GEngine::SceneManager::HasBegun())) {
			GEngine::Application::SetInputEnabled(isFocused);
		}
		ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
		Vector2<float> sz = { viewPortSize.x, viewPortSize.y };
		GEngine::Ref<GEngine::RenderPipeline> pipeline = GEngine::Renderer::GetPipeline(m_pipeline.c_str());
		Vector2<uint32_t> csz = { pipeline->GetFrameBuffer()->GetTexture()->GetWidth(), pipeline->GetFrameBuffer()->GetTexture()->GetHeight() };
		if (lastFrameSize != sz || handleResize) {

			finalSize = scaleRatio((int)sz.x, (int)sz.y, (int)originalSize.x, (int)originalSize.y);
			GEngine::Application::GetApp()->m_viewPortWidth = finalSize.x;
			GEngine::Application::GetApp()->m_viewPortHeight = finalSize.y;
			GEngine::Camera* cam = pipeline->GetCamera();
			if (!cam) {
				cam = GEngine::Application::GetApp()->GetTargetCamera();
			}
			if (cam) {
				const float zoomLevel = m_cameraController ? m_cameraController->GetFOV() : 1.f;
				const float aspectRatio = (float)finalSize.x / (float)finalSize.y;
				cam->SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
				if (m_cameraController)
					m_cameraController->SetAspectRatio(aspectRatio);
			}
		

			lastFrameSize = sz;
			handleResize = false;
		}

		ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - finalSize.x) * .5f , ImGui::GetCursorPosY() });
		GEngine::Application::SetViewPortOffset({ ImGui::GetCursorPosX() + ImGui::GetWindowPos().x, ImGui::GetCursorPosY() + ImGui::GetWindowPos().y });
		ImGui::Image((void*)(intptr_t)pipeline->GetFrameBuffer()->GetTexture()->GetRendererID(), { (float)finalSize.x,(float)finalSize.y }, { 0,1 }, { 1,0 }, { 1,1,1,1 }, {0,0,0,1.f});
		ImGui::EndChild();

		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportModule::Update(GEngine::Timestep timestep)
	{
		bool enabled = Application::InputEnabled();
		if (isFocused) {
			if (!enabled) Application::SetInputEnabled(true);

			if (Application::InputEnabled()) CameraControls(timestep);

			if (!enabled) Application::SetInputEnabled(enabled);
		}
	}

	GEngine::Vector2<uint32_t> ViewportModule::scaleRatio(int maxWidth, int maxHeight, int imgWidth, int imgHeight)
	{
		// calc
		float widthRatio = (float)maxWidth / (float)imgWidth,
			heightRatio = (float)maxHeight / (float)imgHeight;
		float bestRatio = std::min(widthRatio, heightRatio);

		// output
		int newWidth = (int)((float)imgWidth * bestRatio),
			newHeight = (int)((float)imgHeight * bestRatio);
		return { (uint32_t)newWidth, (uint32_t)newHeight };
	}


	void ViewportModule::CameraControls(GEngine::Timestep timestep)
	{
		Vector3f pos = m_cameraController->GetPosition();
		Vector3f npos = pos;

		float zoomPos = m_cameraController->GetFOV();
		float nzooomPos = zoomPos;

		if (Input::IsMouseButtonPressed(GE_SCROLL_DOWN)) {
			zoomPos += cameraSpeed * (float)timestep;
		}

		if (Input::IsMouseButtonPressed(GE_SCROLL_UP)) {
			zoomPos -= cameraSpeed * (float)timestep;
		}

		if (Input::IsKeyDown(GE_KEY_D)) {
			pos.x += cameraSpeed * (float)timestep;
		}
		if (Input::IsKeyDown(GE_KEY_A)) {
			pos.x -= cameraSpeed * (float)timestep;
		}

		if (Input::IsKeyDown(GE_KEY_W)) {
			pos.y += cameraSpeed * (float)timestep;
		}

		if (Input::IsKeyDown(GE_KEY_S)) {
			pos.y -= cameraSpeed * (float)timestep;
		}

		if (Input::IsKeyDown(GE_KEY_F)) {
			pos = Vector3f(0.f);
		}
	


		if (m_cameraController) {
			if (npos != pos) {
				m_cameraController->SetPosition(pos);
				*editorTools = EditorEventType::EditorToolEventDrag;
			}

			if (nzooomPos != zoomPos) {
				m_cameraController->SetCameraZoom(zoomPos);
				*editorTools = EditorEventType::EditorToolEventDrag;
			}			
		}



	}

}
