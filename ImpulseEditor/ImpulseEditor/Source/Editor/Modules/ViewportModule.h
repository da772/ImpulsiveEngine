#pragma once
#include "EditorModule.h"
#include "Editor/EditorLayer.h"


namespace Editor {

	class ReloadModule;

	class ViewportModule : public EditorModule {

	public:
		ViewportModule(const std::string& pipeline, ReloadModule* reloadModule, bool gameView = false, GEngine::CameraController* cam = nullptr, EditorEventType* tool = nullptr, SerializerModule* serializer = nullptr);
		~ViewportModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;
		virtual void Update(GEngine::Timestep timestep) override;


	private:
		GEngine::Vector2<uint32_t> scaleRatio(int, int, int, int);
		bool isFocused = false;
		void CameraControls(GEngine::Timestep t);

	private:
		bool handleResize = false;
		float imageButtonSize = 35.f;
		bool gameView;
		EditorEventType* editorTools;
		GEngine::CameraController* m_cameraController;
		ReloadModule* m_reloadModule;
		SerializerModule* m_serializer;
		std::string m_pipeline;
		GEngine::Vector2<int> originalSize = { 0,0 };
		GEngine::Vector2f lastFrameSize;
		GEngine::Vector2<uint32_t> finalSize = { 0, 0 };
		std::string aspectRatio = "1920x1080";
		float cameraSpeed = 5.f;
		
	};

}