#ifdef GE_EDITOR
#include "ToolbarModule.h"
#include "ReloadModule.h"

#include "Editor/Events/EditorApplicationEvents.h"
#include "Editor/Events/EditorSceneEvents.h"
#include "Editor/Events/EditorToolEvents.h"
#include "Editor/Modules/SerializerModule.h"

namespace Editor {

	ToolbarModule::ToolbarModule(ReloadModule* reloadModule, SerializerModule* serializer, EditorEventType* editorTool)  : m_reloadModule(reloadModule), m_editorTool(editorTool), m_serializer(serializer)
	{
		m_textures["playButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/play172x172.png");
		m_textures["pauseButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/pause172x172.png");
		m_textures["stopButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/stop172x172.png");
		m_textures["resumeButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/resume172x172.png");
		m_textures["fastForwardButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/fastForward172x172.png");
		m_textures["loadingButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/loadingIcon172x172.png");
		m_textures["handButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/hand172x172.png");
		m_textures["scaleButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/scale172x172.png");
		m_textures["rotateButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/rotate172x172.png");
		m_textures["moveButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/drag172x172.png");
		m_textures["undoButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/undo160x160.png");
		m_textures["redoButton"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/redo160x160.png");

		m_eventHash[0] = EditorLayer::GetDispatcher()->SubscribeEvent(EditorEventType::AllEvents, [this](const EditorEvent& e) {

			if (e.GetCategoryFlags() & EditorEventCategory::EventCategoryModification) {
				if (e.GetEventType() == EditorEventType::SceneLoad) {
					m_history.clear();
					m_historyIndex = 0;
				}
				AddHistory();
			}
			else if (e.GetEventType() == EditorEventType::UndoScene) {
				Undo();
			}
			else if (e.GetEventType() == EditorEventType::RedoScene) {
				Redo();
			}

		
		});


	
		char* empty = (char*)malloc(sizeof(char));

		GEngine::Ref<GEngine::FileData> fd = std::make_shared<GEngine::FileData>(sizeof(char), (unsigned char*)empty);
		m_history.push_back(fd);

	}

	ToolbarModule::~ToolbarModule()
	{
		EditorLayer::GetDispatcher()->UnsubscribeEvent(EditorEventType::AllEvents, m_eventHash[0]);
	}

	void ToolbarModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (true)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize({ viewport->Size.x , 55.f });
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
		if (ControlButtons("handButton", EditorEventType::EditorToolEventDrag)) {
			Move();
		}
		ImGui::SameLine(0.f, 15.f);
		
		if (ControlButtons("moveButton", EditorEventType::EditorToolEventMove)) {
			Translate();
		}
		ImGui::SameLine(0.f, 15.f);
		
		if (ControlButtons("rotateButton", EditorEventType::EditorToolEventRotate)) {
			Rotate();
		}
		ImGui::SameLine(0.f, 15.f);
		if (ControlButtons("scaleButton", EditorEventType::EditorToolEventScale)) {
			Scale();
		}

		ImGui::SameLine(0.f, 45.f);
		if (ControlButtons("undoButton", EditorEventType::EditorToolEventUndo, m_history.size() > 1 && m_historyIndex > 0,false)) {
			Undo();
		}

		ImGui::SameLine(0.f, 15.5f);
		if (ControlButtons("redoButton", EditorEventType::EditorToolEventRedo, m_historyIndex != m_history.size()-1 && m_history.size() > 1, false)) {
			Redo();
		}

		
		ImGui::SameLine(0.f, 15.5f);
		if (GEngine::SceneManager::HasBegun()) {

			if (GEngine::Application::IsGamePaused()) {

				ImGui::SetCursorPosX( (ImGui::GetWindowSize().x) * .5f - imageBorderSize * 2);
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
				ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * .5f - imageBorderSize * 2);
				if (ControlButtons("pauseButton")) {
					GEngine::Application::PauseGame();
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPauseEvent>();
				}
				ImGui::SameLine(0.f, 15.f);
			}

			if (ControlButtons("stopButton")) {
				ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * .5f - imageBorderSize * 2);
				const std::string name = GEngine::SceneManager::GetCurrentScene()->GetId();
				if (GEngine::Application::IsGamePaused()) {
					GEngine::Application::ResumeGame();
				}
				GEngine::SceneManager::SetCurrentScene(name.c_str(), false, []() {EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationStopEvent>(); });
				
			}

		}
		else {
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * .5f - imageBorderSize * 2);
			if (m_reloadModule->IsReloading()) {
				ControlButtons("loadingButton");
			}
			else {
				if (ControlButtons("playButton")) {
					EditorLayer::GetDispatcher()->BroadcastEvent<ApplicationPlayEvent>();
					GEngine::SceneManager::Begin();
				}
			}

		}

		ImGui::End();

		ImGui::PopStyleVar(3);


	}

	void ToolbarModule::Undo()
	{
		m_serializer->Load(m_history[--m_historyIndex]);
		EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneHistoryValidationEvent>(m_history.size() > 1 && m_historyIndex > 0, m_historyIndex != m_history.size() - 1 && m_history.size() > 1);
	}

	void ToolbarModule::Redo()
	{
		m_serializer->Load(m_history[++m_historyIndex]);
		EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneHistoryValidationEvent>(m_history.size() > 1 && m_historyIndex > 0, m_historyIndex != m_history.size() - 1 && m_history.size() > 1);
	}

	void ToolbarModule::Move()
	{

	}

	void ToolbarModule::Scale()
	{

	}

	void ToolbarModule::Rotate()
	{

	}

	void ToolbarModule::Translate()
	{

	}

	void ToolbarModule::ClearHistory()
	{
		m_history.clear();
		m_historyIndex = 0;
	}

	void ToolbarModule::AddHistory()
	{
		if (m_history.size() > 0 && m_historyIndex != m_history.size() - 1) {
			m_history.erase(m_history.begin() + m_historyIndex + 1, m_history.end());
		}
		m_history.push_back(m_serializer->SerializeCurrentScene());
		m_historyIndex = m_history.size() - 1;
		if (m_history.size() >= 15) {
			m_history.erase(m_history.begin());
			m_historyIndex = m_history.size() - 1;
		}

		EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneHistoryValidationEvent>(m_history.size() > 1 && m_historyIndex > 0, m_historyIndex != m_history.size() - 1 && m_history.size() > 1);
	}


	bool ToolbarModule::ControlButtons(const std::string& s, EditorEventType tool, bool enabled, bool setTool)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		bool selected = tool != EditorEventType::None && tool == *m_editorTool;
		bool isHovered = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect({ pos.x-imageBorderSize*.25f, pos.y - (imageBorderSize-imageButtonSize)/2.f }, { pos.x + imageBorderSize + imageBorderSize * .25f, pos.y + imageBorderSize });
		ImU32 col = ImGui::GetColorU32(ImGui::GetStyleColorVec4(!enabled ? ImGuiCol_TitleBg : selected ? ImGuiCol_ButtonActive : !isHovered || !enabled ? ImGuiCol_Button : ImGuiCol_ButtonHovered));
		if (!enabled) {
			col &= 0x00ffffff;
			col |= 0xff000000;
		}
		ImVec4 v4 = ImGui::ColorConvertU32ToFloat4(col);
		ImGui::GetWindowDrawList()->AddRectFilled({ pos.x-imageBorderSize * .25f, pos.y - (imageBorderSize - imageButtonSize) / 2.f }, { pos.x + imageBorderSize + imageBorderSize * .25f, pos.y + imageBorderSize}, col, 0.f);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (imageBorderSize - imageButtonSize) / 2.f);
		ImGui::Image((ImTextureID)(uintptr_t)m_textures[s]->GetRendererID(), { imageButtonSize, imageButtonSize}, { 0,1 }, { 1,0 });
		if (isHovered && enabled) {
			bool clicked = ImGui::IsMouseClicked(0);
			if (tool != EditorEventType::None && clicked) {
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorToolEvent>(tool);
				if (setTool)
					*m_editorTool = tool;
			}
			return clicked;
		}
		return false;
	}

}
#endif