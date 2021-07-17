#include "EditorScene.h"

using namespace GEngine;

namespace Editor {


	EditorScene::EditorScene(const char* id, Camera* camera) : Scene(id, camera)
	{
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWidth() / (float)GEngine::Application::GetApp()->GetHeight()));

		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		GEngine::Application::GetApp()->SetTargetCameraController(m_CameraController.get());


	//	Entity* e = CreateEntity<Entity>();
		//SpriteComponent* s = e->AddComponent<SpriteComponent>();
		//s->CreateQuad({ 20,0,0 }, 0, { 20,20,1 }, { 1,0,1,1 });


	}

	void EditorScene::OnEvent(Event& e)
	{

	}

	void EditorScene::OnUpdate(GEngine::Timestep timestep)
	{
		m_CameraController->OnUpdate(timestep);
		
		//GE_LOG_DEBUG("UPDATING: {0}", timestep);
	}

	void EditorScene::OnBegin()
	{
		
		GE_LOG_INFO("INFO");
		GE_LOG_TRACE("TRACE");
		GE_LOG_DEBUG("DEBUG");
		GE_LOG_WARN("WARN");
		GE_LOG_ERROR("ERROR");
		//textComponent->CreateQuad({ 0,0,2 }, 0, { 2.5,2,1 }, { 1,1,1,1.f }, Texture2D::Create("Content/Textures/sky_01.png"));

	}

	void EditorScene::OnEnd()
	{
	}

}
