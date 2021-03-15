#include "gepch.h"

#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Collision/CollisionDetection.h"


namespace GEngine {

	std::unordered_map<std::string, std::function<Scene*()>> SceneManager::scenes;
	Scene* SceneManager::scene = nullptr;
	std::vector<std::function<void()>> SceneManager::m_FlushFunctions;
	bool SceneManager::b_started = false;
	bool SceneManager::b_paused = false;



	Scene* SceneManager::GetCurrentScene()
	{
		return SceneManager::scene;
	}

	void SceneManager::SetCurrentScene(const std::string& name)
	{
		ThreadPool::AddEndFrameFunction([name]() {
			CollisionDetection::Reset();
			if (SceneManager::scene != nullptr) {
				scene->End();
				SceneManager::m_FlushFunctions.clear();
				delete SceneManager::scene;
				SceneManager::scene = nullptr;
			}

			if (SceneManager::scenes.find(name.c_str()) != SceneManager::scenes.end()) {
				SceneManager::scene = SceneManager::scenes[name]();
				Begin();
			}
			});
	}

	void SceneManager::Update(Timestep ts)
	{
		if (scene != nullptr && !b_paused) {
			scene->Update(ts);
		}
	}

	void SceneManager::Begin()
	{
		if (scene != nullptr) {
			SceneManager::b_started = true;
			scene->Begin();
		}
	}

	void SceneManager::End()
	{
		if (scene != nullptr) {
			SceneManager::b_started = false;
			scene->End();
		}
		SceneManager::m_FlushFunctions.clear();
	}

	void SceneManager::ReloadGraphics(){
		if (scene != nullptr) {
			CollisionDetection::Reset();
			scene->ReloadGraphics();
		}
	}

	void SceneManager::UnloadGraphics() {
		if (scene != nullptr) {
			CollisionDetection::Reset();
			scene->UnloadGraphics();
		}
	}

	void SceneManager::Shutdown()
	{
		End();
		if (scene) {
			delete scene;
			scene = nullptr;
		}
	}

	void SceneManager::OnEvent(Event& e)
	{
		if (scene != nullptr && !b_paused && scene->IsInitalized()) {
			scene->OnEvent(e);
		}
	}

	glm::mat4 SceneManager::GetCurrentViewProjectionMatrix()
	{
		if (scene != nullptr) {
			return scene->GetCamera()->GetViewProjectionMatrix();
		}
		return glm::mat4();
	}

	void SceneManager::ImGuiRender()
	{
		if (scene != nullptr && !b_paused)
			scene->OnImGuiRender();
	}

	void SceneManager::Pause(bool b)
	{
		CollisionDetection::Reset();
		b_paused = b;
	}

	void SceneManager::ResetScene()
	{
		if (scene != nullptr) {
			CollisionDetection::Reset();
			End();
			scene->Unload();
			Begin();
		}
	}

}
