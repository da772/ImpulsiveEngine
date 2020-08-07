#pragma once
#include "Public/Core/Core.h"

namespace GEngine {

	class Scene;
	class Event;
	struct Timestep;

	class SceneManager {
	
	public:
		static void AddScene(std::string name, Ref<Scene> scene);
		static Ref<Scene> GetCurrentScene();
		static void SetCurrentScene(std::string name, bool unload = true);
		static void Update(Timestep ts);
		static void Begin();
		static void End();
		static Ref<Scene> GetScene(const char* name);
		static void OnEvent(Event& e);
		static glm::mat4 GetCurrentViewProjectionMatrix();
		static void ImGuiRender();
		static void Pause(bool b);
		static void ResetScene();


	private:
		static std::unordered_map<std::string, Ref<Scene>> scenes;
		static Ref<Scene> scene;
		static std::vector < std::function<void()> > m_FlushFunctions;
		static bool b_started;
		static bool b_paused;

	};

}

