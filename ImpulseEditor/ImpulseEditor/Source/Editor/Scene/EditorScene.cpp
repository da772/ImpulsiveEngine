#include "EditorScene.h"

using namespace GEngine;

namespace Editor {


	EditorScene::EditorScene(const char* id, Camera* camera) : Scene(id, camera)
	{
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWindow()->GetWidth() / (float)GEngine::Application::GetApp()->GetWindow()->GetHeight()));
		buttonTexture = GEngine::Texture2D::Create("Content/Textures/Icons/fileIcon256x256.png");

		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);

		buttonsEntity = CreateEntity<Entity>();
		textComponent = buttonsEntity->AddComponent<UIComponent>();

		float size = std::max((float)Application::GetWidth() / (float)Application::GetUIResolutionWidth(), (float)Application::GetHeight() / (float)Application::GetUIResolutionHeight());
		float buttonY = size * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 5.f;
		float buttonX = size * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 5.f;

		startButton = buttonsEntity->AddComponent<ButtonComponent>(Vector3f(0, 0, 10), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		startButton->SetImageTexture(buttonTexture);

	}

	void EditorScene::OnEvent(Event& e)
	{
		
	}

	void EditorScene::OnUpdate(GEngine::Timestep timestep)
	{
		GE_LOG_DEBUG("UPDATING: {0}", timestep);
	}

	void EditorScene::OnBegin()
	{
		
		startButton->SetImageColor({ 1,0,0,1 });
		//textComponent->CreateQuad({ 0,0,2 }, 0, { 2.5,2,1 }, { 1,1,1,1.f }, Texture2D::Create("Content/Textures/sky_01.png"));

	}

	void EditorScene::OnEnd()
	{
		buttonsEntity = nullptr;
		textComponent = nullptr;
		startButton = nullptr;
	}

}