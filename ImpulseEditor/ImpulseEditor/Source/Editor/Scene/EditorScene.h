#pragma once

#include <GEngine.h>

namespace Editor {


	class EditorScene : public GEngine::Scene {

	public:
		EditorScene(const char* id, GEngine::Camera* camera);
		void OnEvent(GEngine::Event& e) override;


		void OnUpdate(GEngine::Timestep timestep) override;


		void OnBegin() override;


		void OnEnd() override;
	private:
		GEngine::Entity* buttonsEntity = nullptr;
		GEngine::UIComponent* textComponent = nullptr;
		GEngine::ButtonComponent* startButton = nullptr;
		GEngine::SpriteComponent* background = nullptr;
		GEngine::Ref<GEngine::Texture2D> buttonTexture = nullptr;
		GEngine::Scope<GEngine::Orthographic_CameraController> m_CameraController;
		float r = 1;
		float g = 0;
		float b = 0;

	};
	

}
