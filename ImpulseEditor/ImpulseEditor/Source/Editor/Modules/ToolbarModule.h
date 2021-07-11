#pragma once
#include "EditorModule.h"


namespace Editor {

	class ReloadModule;

	class ToolbarModule : public EditorModule {

	public:
		ToolbarModule(ReloadModule* reloadModule);
		~ToolbarModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;


	private:
		bool ControlButtons(const std::string& s);
	private:
		ReloadModule* m_reloadModule;
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		float imageButtonSize = 25.f;
		float yOffset = 1.f;

	};

}