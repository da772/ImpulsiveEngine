#include "gepch.h"

#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Collision/CollisionDetection.h"


namespace GEngine {

	std::unordered_map<std::string, Ref<Scene>> SceneManager::scenes;
	Ref<Scene> SceneManager::scene;
	std::vector<std::function<void()>> SceneManager::m_FlushFunctions;
	bool SceneManager::b_started = false;
	bool SceneManager::b_paused = false;


	void SceneManager::AddScene(const std::string& name, Ref<Scene> scene)
	{
		SceneManager::scenes[name] = scene;
	}

	GEngine::Ref<Scene> SceneManager::GetCurrentScene()
	{
		return SceneManager::scene;
	}

	void SceneManager::SetCurrentScene(const std::string& name, bool unload)
	{
		ThreadPool::AddEndFrameFunction([name, unload]() {
			CollisionDetection::Reset();
			if (SceneManager::scene != nullptr) {
				scene->End();
				if (unload)
					scene->Unload();
				SceneManager::m_FlushFunctions.clear();
			}

			if (SceneManager::scenes.find(name.c_str()) != SceneManager::scenes.end()) {
				SceneManager::scene = SceneManager::scenes[name];
				if (SceneManager::b_started)
					scene->Begin();
				else
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
	


	GEngine::Ref<GEngine::Scene> SceneManager::GetScene(const char* name)
	{
		if (SceneManager::scenes.find(name) != SceneManager::scenes.end())
			return SceneManager::scenes[name];
		else
			return nullptr;
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
