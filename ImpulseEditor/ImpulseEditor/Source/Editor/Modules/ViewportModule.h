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

	private:
		bool handleResize = false;
		std::string m_pipeline;
		GEngine::Vector2<int> originalSize = { 0,0 };
		GEngine::Vector2f lastFrameSize;
		GEngine::Vector2<uint32_t> finalSize = { 0, 0 };
		
	};

}