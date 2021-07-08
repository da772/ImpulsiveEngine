#include "EditorScene.h"

using namespace GEngine;

namespace Editor {


	EditorScene::EditorScene(const char* id, Camera* camera) : Scene(id, camera)
	{
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWindow()->GetWidth() / (float)GEngine::Application::GetApp()->GetWindow()->GetHeight()));
		buttonTexture = GEngine::Texture2D::Create("EngineContent/Textures/Icons/fileIcon256x256.png");

		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		auto eChild = CreateEntity<Entity>();
		CreateEntity<Entity>();
		buttonsEntity = CreateEntity<Entity>();
		buttonsEntity->SetTag("TestEntity");
		textComponent = buttonsEntity->AddComponent<UIComponent>();
		background = buttonsEntity->AddComponent<SpriteComponent>();
		buttonsEntity->AddChild(eChild);
		buttonsEntity->AddChild(CreateEntity<Entity>());
		eChild->AddChild(CreateEntity<Entity>());
		buttonsEntity->AddComponent<NativeScriptComponent>("");
		background->CreateQuad({ 0,0,0 }, 0, { 10,10,10 }, { 0, 0,1,1 });

		float size = std::max((float)Application::GetWidth() / (float)Application::GetUIResolutionWidth(), (float)Application::GetHeight() / (float)Application::GetUIResolutionHeight());
		float buttonY = size * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 3.f;
		float buttonX = size * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 3.f;

		

		startButton = buttonsEntity->AddComponent<ButtonComponent>(Vector3f(0, 0, 10), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		startButton->SetImageTexture(buttonTexture);

	}

	void EditorScene::OnEvent(Event& e)
	{
		
		if (e.GetEventType() == EventType::MouseMoved) {
			if (Application::InputEnabled()) {
				MouseMovedEvent& mouse = static_cast<MouseMovedEvent&>(e);
				GE_CORE_DEBUG("MOUSE MOVED: {0}, {1}", mouse.GetX() - Application::GetViewPortOffsetX(), mouse.GetY() - Application::GetViewPortOffsetY());
			}
		}
	}

	void EditorScene::OnUpdate(GEngine::Timestep timestep)
	{
		startButton->SetImageColor({ ((int)r %255)/255.f,((int)g%255)/255.f,((int)b%255)/255.f,1 });

		r += 125.f * timestep;
		g += 100.f * timestep;
		b += 75.f * timestep;

		//GE_LOG_DEBUG("UPDATING: {0}", timestep);
	}

	void EditorScene::OnBegin()
	{
		
		startButton->SetImageColor({ 1,0,0,1 });
		GE_LOG_INFO("INFO");
		GE_LOG_TRACE("TRACE");
		GE_LOG_DEBUG("DEBUG");
		GE_LOG_WARN("WARN");
		GE_LOG_ERROR("ERROR");
		//textComponent->CreateQuad({ 0,0,2 }, 0, { 2.5,2,1 }, { 1,1,1,1.f }, Texture2D::Create("Content/Textures/sky_01.png"));

	}

	void EditorScene::OnEnd()
	{
		buttonsEntity = nullptr;
		textComponent = nullptr;
		startButton = nullptr;
	}

}