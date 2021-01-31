#pragma once

#include <GEngine.h>

using namespace GEngine;

class MapEditor : public Scene {

public:
	MapEditor(const char* id, Camera* camera);
	~MapEditor();
	void OnEvent(Event& e) override;
	void OnUpdate(Timestep timestep) override;
	void OnBegin() override;
	void OnEnd() override;
	void OnImGuiRender() override;

	void OnLoad() override;


	void OnUnload() override;


	static unordered_map<std::string, std::function<Ref<GameObject>()>> entityMap;

private:
	GEngine::Scope<GEngine::Orthographic_CameraController> m_CameraController;
	void SetupCamera();
	void SceneMenu();
	void SaveScene(const std::string& location);
	void LoadScene(const std::string& scene);

};
