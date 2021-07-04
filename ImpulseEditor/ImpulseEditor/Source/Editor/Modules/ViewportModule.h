#pragma once
#include "EditorModule.h"

namespace Editor {

	class ViewportModule : public EditorModule {

		public:
			ViewportModule(const std::string& pipeline);
			~ViewportModule();

			virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;
	private:
		GEngine::Vector2<uint32_t> scaleRatio(int, int, int, int);
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;

		bool ControlButtons(const std::string& s);

	private:
		bool handleResize = false;
		float imageButtonSize = 35.f;
		std::string m_pipeline;
		GEngine::Vector2<int> originalSize = { 0,0 };
		GEngine::Vector2f lastFrameSize;
		GEngine::Vector2<uint32_t> finalSize = { 0, 0 };
		std::string aspectRatio = "1920x1080";
		
	};

}