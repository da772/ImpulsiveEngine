#pragma once
#include "EditorModule.h"
#include "Editor/EditorLayer.h"

namespace Editor {

	class ReloadModule;
	class SerializerModule;

	class ToolbarModule : public EditorModule {

	public:
		ToolbarModule(ReloadModule* reloadModule, SerializerModule* serializer, EditorEventType* editorTool);
		~ToolbarModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;
		void Undo();
		void Redo();
		void Move();
		void Scale();
		void Rotate();
		void Translate();
		void ClearHistory();
		void AddHistory();

	private:
		bool ControlButtons(const std::string& s, EditorEventType tool = EditorEventType::None, bool enabled = true, bool setTool = true);
	private:
		ReloadModule* m_reloadModule;
		EditorEventType* m_editorTool;
		SerializerModule* m_serializer;
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		float imageButtonSize = 18.5f;
		float imageBorderSize = 21.0f;
		float yOffset = 2.f;

		std::vector<GEngine::Ref<GEngine::FileData>> m_history;
		uint32_t m_historyIndex = 0;
		uint64_t m_eventHash[1] = { 0 };

		uint32_t m_maxHistory = 15;

	};

}