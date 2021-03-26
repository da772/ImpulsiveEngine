#pragma once
#include "Public/Core/Core.h"

namespace GEngine {

	class Scene;
	class Event;
	struct Timestep;

	class GE_API SceneManager {
	
	public:
		template<typename S = Scene>
		static inline void AddScene(const std::string& name) {
			SceneManager::scenes[name] = [name]() {
				S* s = new S(name.c_str(), nullptr);
				return s;
			};
		};
		static Scene* GetCurrentScene();
		static void SetCurrentScene(const std::string& name);
		static void Update(Timestep ts);
		static void Begin();
		static void End();
		static void OnEvent(Event& e);
		static glm::mat4 GetCurrentViewProjectionMatrix();
		static void ImGuiRender();
		static void Pause(bool b);
		static void ResetScene();
		static void ReloadGraphics();
		static void UnloadGraphics();
		static void Shutdown();

	private:
		static std::unordered_map<std::string, std::function<Scene*()>> scenes;
		static Scene* scene;
		static std::vector < std::function<void()> > m_FlushFunctions;
		static bool b_started;
		static bool b_paused;
		static bool b_changingScene;

	};

}

