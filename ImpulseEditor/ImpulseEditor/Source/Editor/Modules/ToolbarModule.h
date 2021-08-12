#pragma once
#include "EditorModule.h"
#include "Editor/EditorLayer.h"

namespace Editor {

	class ReloadModule;


	class ToolbarModule : public EditorModule {

	public:
		ToolbarModule(ReloadModule* reloadModule, EditorEventType* editorTool);
		~ToolbarModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;


	private:
		bool ControlButtons(const std::string& s, EditorEventType tool = EditorEventType::None, bool setTool = true);
	private:
		ReloadModule* m_reloadModule;
		EditorEventType* m_editorTool;
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		float imageButtonSize = 18.5f;
		float imageBorderSize = 21.0f;
		float yOffset = 2.f;

	};

}