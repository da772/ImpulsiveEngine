#pragma once
#include "EditorModule.h"


namespace Editor {
	
	class InfoPanelModule : public EditorModule {

	public:
		InfoPanelModule();
		~InfoPanelModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	private:
		float offset = 50.f, size = 50.f;
		float fontOffset = 5.f;
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;

	};

}